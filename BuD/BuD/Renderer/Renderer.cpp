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

		InitializeBuffers(width, height);
	}

	void Renderer::Dispose()
	{
		s_MainRTV.Reset();
		s_DepthBuffer.Reset();
		s_MainRTVTexture.Reset();

		s_Device.reset();
	}

	void Renderer::BeginFrame()
	{
		if (!s_Device)
		{
			Log::WriteWarning(L"Attempting to begin frame without initializing the renderer.");
			return;
		}

		FLOAT clearColor[] = { sinf(Clock::Now()), 0.0f, 0.0f, 1.0f};

		auto& context = s_Device->Context();
		context->OMSetRenderTargets(1, s_MainRTV.GetAddressOf(), s_DepthBuffer.Get());

		context->ClearRenderTargetView(s_MainRTV.Get(), clearColor);
		context->ClearDepthStencilView(s_DepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

		auto& swapchain = s_Device->Swapchain();
		swapchain->Present(0, 0);
	}

	void Renderer::UpdateBuffersSize(int width, int height)
	{
		s_Device->UpdateSize(width, height);

		auto& context = s_Device->Context();
		auto& swapchain = s_Device->Swapchain();

		context->OMSetRenderTargets(0, nullptr, nullptr);

		s_MainRTVTexture.Reset();
		s_MainRTV.Reset();
		s_DepthBuffer.Reset();

		context->Flush();

		auto hr = swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		InitializeBuffers(width, height);
	}
	
	void Renderer::InitializeBuffers(int width, int height)
	{
		auto& device = s_Device->Device();
		auto& context = s_Device->Context();
		auto& swapchain = s_Device->Swapchain();

		s_Device->UpdateSize(width, height);

		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)s_MainRTVTexture.GetAddressOf());

		s_MainRTV = s_Device->CreateRenderTargetView(s_MainRTVTexture);
		s_DepthBuffer = s_Device->CreateDepthStencilBuffer(width, height);

		context->OMSetRenderTargets(1, s_MainRTV.GetAddressOf(), s_DepthBuffer.Get());

		ViewportDesc viewportDesc{ static_cast<UINT>(width), static_cast<UINT>(height) };
		context->RSSetViewports(1, &viewportDesc);
	}
}
