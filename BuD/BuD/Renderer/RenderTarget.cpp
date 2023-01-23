#include "bud_pch.h"
#include "RenderTarget.h"

#include "Structures/Texture2DDesc.h"

namespace BuD
{
	RenderTarget::RenderTarget(std::shared_ptr<GraphicsDevice> graphicsDevice, UINT width, UINT height)
		: Width(width), Height(height), FramesWithoutUsage(0)
	{
		auto& device = graphicsDevice->Device();
		auto& context = graphicsDevice->Context();

		Texture2DDesc desc(width, height);
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

		TargetTexture = graphicsDevice->CreateTexture(desc);
		RenderTargetView = graphicsDevice->CreateRenderTargetView(TargetTexture);
		DepthStencilView = graphicsDevice->CreateDepthStencilBuffer(width, height);

		auto hr = device->CreateShaderResourceView(TargetTexture.Get(), nullptr, ShaderResourceView.GetAddressOf());

		if (FAILED(hr))
		{
			Log::WriteError(L"Error while creating SRV for render target");
		}
	}
	
	RenderTarget::~RenderTarget()
	{
		DepthStencilView.Reset();
		ShaderResourceView.Reset();
		RenderTargetView.Reset();
		TargetTexture.Reset();
	}
}
