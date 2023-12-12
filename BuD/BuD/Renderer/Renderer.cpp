#include "bud_pch.h"
#include "Renderer.h"

#include <Utils/Log.h>
#include <Utils/Clock.h>

#include "Implementations/AnaglyphRendererImpl.h"
#include "Implementations/StandardRendererImpl.h"
#include "Structures/Texture2DDesc.h"
#include "Structures/ViewportDesc.h"

namespace BuD
{
	void Renderer::Initialize(std::shared_ptr<Win32Window> window)
	{
		s_Device = std::make_shared<GraphicsDevice>(window);

		auto standardImpl = std::make_shared<StandardRendererImpl>(s_Device);
		auto anaglyphImpl = std::make_shared<AnaglyphRendererImpl>(s_Device);

		s_RenderingImplementations.emplace(RenderingMode::STANDARD, standardImpl);
		s_RenderingImplementations.emplace(RenderingMode::ANAGLYPH, anaglyphImpl);

		s_MultiEyeRendererImplementations.insert(anaglyphImpl);

		s_ActiveRendererImpl = s_RenderingImplementations.at(s_RenderingMode);

		auto width = window->Width();
		auto height = window->Height();

		InitializeSwapchainBuffers(width, height);
	}

	void Renderer::Dispose()
	{
		s_MainRTV.Reset();
		s_DepthBuffer.Reset();
		s_MainRTVTexture.Reset();

		s_Device.reset();
	}

	void Renderer::BeginTarget(int width, int height)
	{
		auto result = std::find_if(s_FreeRenderTargets.rbegin(), s_FreeRenderTargets.rend(),
			[width, height](std::shared_ptr<RenderTarget> rt) { return rt->Width == width && rt->Height == height; }
		);

		if (result != s_FreeRenderTargets.rend())
		{
			auto& renderTargetPointer = *result;
			renderTargetPointer->FramesWithoutUsage = 0;

			s_ActiveRenderTargets.push(renderTargetPointer);
			s_FreeRenderTargets.erase(std::find(s_FreeRenderTargets.begin(), s_FreeRenderTargets.end(), renderTargetPointer));
		}
		else
		{
			s_ActiveRenderTargets.push(std::make_shared<RenderTarget>(s_Device, width, height));
		}

		RenderToExternalTarget();
	}

	Texture Renderer::EndTarget()
	{
		if (s_ActiveRenderTargets.empty())
		{
			Log::WriteError("Attempting to retrieve a render target without starting it beforehand.");
			return { nullptr };
		}

		std::shared_ptr<RenderTarget> activeRenderTarget = s_ActiveRenderTargets.top();
		Texture result(activeRenderTarget->ShaderResourceView);

		s_ActiveRenderTargets.pop();
		s_UsedRenderTargets.push_back(activeRenderTarget);

		if (s_ActiveRenderTargets.empty())
		{
			RenderToSwapchain();
		}
		else
		{
			RenderToExternalTarget();
		}

		return result;
	}

	float Renderer::AspectRatio()
	{
		if (s_RenderingToSwapchain)
		{
			return static_cast<float>(s_Device->BufferWidth()) / static_cast<float>(s_Device->BufferHeight());
		}

		auto& target = s_ActiveRenderTargets.top();

		return static_cast<float>(target->Width) / static_cast<float>(target->Height);
	}

	void Renderer::UseLeftHandCoordinateSystem()
	{
		for (auto& [renderingMode, implementation] : s_RenderingImplementations)
		{
			implementation->UseLeftHandCoordinateSystem();
		}
	}

	void Renderer::UseRightHandCoordinateSystem()
	{
		for (auto& [renderingMode, implementation] : s_RenderingImplementations)
		{
			implementation->UseRightHandCoordinateSystem();
		}
	}

	bool Renderer::IsMultiEyeMode()
	{
		return std::dynamic_pointer_cast<MultiEyeRendererImpl>(s_ActiveRendererImpl) != std::shared_ptr<MultiEyeRendererImpl>();
	}

	RenderingMode Renderer::GetRenderingMode()
	{
		return s_RenderingMode;
	}

	void Renderer::SetRenderingMode(RenderingMode mode)
	{
		if (mode == s_RenderingMode)
		{
			return;
		}

		s_ActiveRendererImpl = s_RenderingImplementations[mode];
		s_RenderingMode = mode;
	}

	MultiEyeSettings Renderer::GetMultiEyeSettings()
	{
		return s_MultiEyeSettings;
	}

	void Renderer::SetMultiEyeSettings(const MultiEyeSettings& settings)
	{
		s_MultiEyeSettings = settings;

		for (auto& multiEyeRendererImplementation : s_MultiEyeRendererImplementations)
		{
			multiEyeRendererImplementation->SetMultiEyeSettings(settings);
		}
	}

	dxm::Matrix Renderer::ProjectionMatrix()
	{
		return s_ActiveRendererImpl->ProjectionMatrix();
	}

	void Renderer::BeginFrame()
	{
		if (!s_Device)
		{
			Log::WriteWarning("Attempting to begin frame without initializing the renderer.");
			return;
		}

		// Release all render targets that have not been used for more than 5 consecutive frames
		constexpr int MAX_FRAMES_WITHOUT_USAGE = 5;
		
		std::erase_if(s_FreeRenderTargets,
			[MAX_FRAMES_WITHOUT_USAGE](std::shared_ptr<RenderTarget> renderTarget) 
			{ 
				return ++renderTarget->FramesWithoutUsage > MAX_FRAMES_WITHOUT_USAGE; 
			}
		);

		// Set all render targets to be free
		std::copy(s_UsedRenderTargets.begin(), s_UsedRenderTargets.end(), std::back_inserter(s_FreeRenderTargets));
		s_UsedRenderTargets.clear();

		while (!s_ActiveRenderTargets.empty())
		{
			s_FreeRenderTargets.push_back(s_ActiveRenderTargets.top());
			s_ActiveRenderTargets.pop();
		}

		RenderToSwapchain();

		Clear(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		auto& context = s_Device->Context();
		FLOAT clearColor[] = { r, g, b, a };

		if (s_RenderingToSwapchain)
		{
			context->ClearRenderTargetView(s_MainRTV.Get(), clearColor);
			context->ClearDepthStencilView(s_DepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
		else
		{
			auto& renderTarget = s_ActiveRenderTargets.top();

			if (renderTarget->RenderTargetView)
				context->ClearRenderTargetView(renderTarget->RenderTargetView.Get(), clearColor);

			if (renderTarget->DepthStencilView)
				context->ClearDepthStencilView(renderTarget->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void Renderer::Render(Scene& scene)
	{
		if (!s_Device)
		{
			Log::WriteWarning("Attempting to render scene without initializing the renderer.");
			return;
		}

		RenderTargetInfo renderTarget;

		if (s_RenderingToSwapchain)
		{
			renderTarget.RenderTargetView = s_MainRTV;
			renderTarget.DepthStencilView = s_DepthBuffer;
			renderTarget.Width = s_Device->BufferWidth();
			renderTarget.Height = s_Device->BufferHeight();
		}
		else
		{
			auto& activeRenderTarget = s_ActiveRenderTargets.top();

			renderTarget.RenderTargetView = activeRenderTarget->RenderTargetView;
			renderTarget.DepthStencilView = activeRenderTarget->DepthStencilView;
			renderTarget.Width = activeRenderTarget->Width;
			renderTarget.Height = activeRenderTarget->Height;
		}

		s_PerformanceData = s_ActiveRendererImpl->Render(scene, renderTarget);
	}

	void Renderer::EndFrame()
	{
		if (!s_Device)
		{
			Log::WriteWarning("Attempting to end frame without initializing the renderer.");
			return;
		}

		if (!s_ActiveRenderTargets.empty())
		{
			Log::WriteWarning("Not all render targets for this frame have been used");
		}

		if (!s_RenderingToSwapchain)
		{
			RenderToSwapchain();
		}

		auto& swapchain = s_Device->Swapchain();
		swapchain->Present(0, 0);
	}

	void Renderer::WindowResized(int width, int height)
	{
		auto& context = s_Device->Context();
		auto& swapchain = s_Device->Swapchain();

		context->OMSetRenderTargets(0, nullptr, nullptr);

		s_MainRTVTexture.Reset();
		s_MainRTV.Reset();
		s_DepthBuffer.Reset();

		context->Flush();

		auto hr = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		InitializeSwapchainBuffers(width, height);
	}
	
	void Renderer::InitializeSwapchainBuffers(int width, int height)
	{
		auto& device = s_Device->Device();
		auto& context = s_Device->Context();
		auto& swapchain = s_Device->Swapchain();

		s_Device->UpdateSize(width, height);

		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)s_MainRTVTexture.GetAddressOf());

		s_MainRTV = s_Device->CreateRenderTargetView(s_MainRTVTexture);
		s_DepthBuffer = s_Device->CreateDepthStencilBuffer(width, height);

		RenderToSwapchain();
	}

	void Renderer::RenderToSwapchain()
	{
		auto& context = s_Device->Context();
		context->OMSetRenderTargets(1, s_MainRTV.GetAddressOf(), s_DepthBuffer.Get());

		ViewportDesc viewportDesc{ s_Device->BufferWidth(), s_Device->BufferHeight() };
		context->RSSetViewports(1, &viewportDesc);

		s_RenderingToSwapchain = true;
	}

	void Renderer::RenderToExternalTarget()
	{
		auto& renderTarget = s_ActiveRenderTargets.top();
		auto& context = s_Device->Context();
		context->OMSetRenderTargets(1, renderTarget->RenderTargetView.GetAddressOf(), renderTarget->DepthStencilView.Get());

		ViewportDesc viewportDesc{ renderTarget->Width, renderTarget->Height };
		context->RSSetViewports(1, &viewportDesc);

		s_RenderingToSwapchain = false;
	}
}
