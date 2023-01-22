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

		FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f};

		auto& context = s_Device->Context();

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
	
	void Renderer::InitializeBuffers(int width, int height)
	{
		auto& device = s_Device->Device();
		auto& context = s_Device->Context();
		auto& swapchain = s_Device->Swapchain();

		s_Device->UpdateSize(width, height);

		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)s_MainRTVTexture.GetAddressOf());

		ComPtr<ID3D11Texture2D> idTexture;
		auto texDesc = Texture2DDesc(width, height);
		texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		
		auto hr = device->CreateTexture2D(&texDesc, nullptr, idTexture.GetAddressOf());

		s_MainRTV = s_Device->CreateRenderTargetView(s_MainRTVTexture);
		s_DepthBuffer = s_Device->CreateDepthStencilBuffer(width, height);

		context->OMSetRenderTargets(1, s_MainRTV.GetAddressOf(), s_DepthBuffer.Get());

		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		s_MainRTVTexture->GetDesc(&backBufferDesc);

		ViewportDesc viewportDesc{ backBufferDesc.Width, backBufferDesc.Height };
		s_Device->Context()->RSSetViewports(1, &viewportDesc);

		idTexture.Reset();
	}
}
