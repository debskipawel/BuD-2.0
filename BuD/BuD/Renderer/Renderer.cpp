#include "bud_pch.h"
#include "Renderer.h"

#include <Utils/Log.h>
#include <Utils/Clock.h>

#include "Structures/Texture2DDesc.h"
#include "Structures/ViewportDesc.h"

namespace BuD
{
	void Renderer::Initialize(std::shared_ptr<Win32Window> window)
	{
		s_Device = std::make_shared<GraphicsDevice>(window);

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

	void Renderer::StartTarget(int width, int height)
	{
		auto result = std::find_if(s_FreeRenderTargets.rbegin(), s_FreeRenderTargets.rend(),
			[width, height](std::shared_ptr<RenderTarget> rt) { return rt->Width == width && rt->Height == height; }
		);

		if (result != s_FreeRenderTargets.rend())
		{
			auto& renderTargetPointer = *result;
			renderTargetPointer->FramesWithoutUsage = 0;

			s_UsedRenderTargets.push_back(renderTargetPointer);
			s_FreeRenderTargets.erase(std::find(s_FreeRenderTargets.begin(), s_FreeRenderTargets.end(), renderTargetPointer));
		}
		else
		{
			s_UsedRenderTargets.push_back(std::make_shared<RenderTarget>(s_Device, width, height));
		}

		RenderToExternalTarget();
	}

	ComPtr<ID3D11ShaderResourceView> Renderer::GetTarget()
	{
		RenderToSwapchain();

		auto& activeRenderTarget = *s_UsedRenderTargets.rbegin();

		return activeRenderTarget->ShaderResourceView;
	}

	void Renderer::BeginFrame()
	{
		if (!s_Device)
		{
			Log::WriteWarning(L"Attempting to begin frame without initializing the renderer.");
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
			auto& renderTarget = *s_UsedRenderTargets.rbegin();

			context->ClearRenderTargetView(renderTarget->RenderTargetView.Get(), clearColor);
			context->ClearDepthStencilView(renderTarget->DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	void Renderer::Render(const Scene& scene)
	{
		if (!s_Device)
		{
			Log::WriteWarning(L"Attempting to render scene without initializing the renderer.");
			return;
		}
	}

	void Renderer::EndFrame()
	{
		if (!s_Device)
		{
			Log::WriteWarning(L"Attempting to end frame without initializing the renderer.");
			return;
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
		auto& renderTarget = *s_UsedRenderTargets.rbegin();
		auto& context = s_Device->Context();
		context->OMSetRenderTargets(1, renderTarget->RenderTargetView.GetAddressOf(), renderTarget->DepthStencilView.Get());

		ViewportDesc viewportDesc{ renderTarget->Width, renderTarget->Height };
		context->RSSetViewports(1, &viewportDesc);

		s_RenderingToSwapchain = false;
	}
}
