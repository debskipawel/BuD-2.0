#include "bud_pch.h"
#include "RenderTarget.h"

#include "Structures/Texture2DDesc.h"

namespace BuD
{
	RenderTarget::RenderTarget(std::shared_ptr<GraphicsDevice> graphicsDevice, int width, int height)
		: Width(std::max(0, width)), Height(std::max(0, height)), FramesWithoutUsage(0)
	{
		if (Width == 0 || Height == 0)
		{
			RenderTargetView = nullptr;
			DepthStencilView = nullptr;

			return;
		}

		auto& device = graphicsDevice->Device();
		auto& context = graphicsDevice->Context();

		Texture2DDesc desc(width, height);
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;

		TargetTexture = graphicsDevice->CreateTexture(desc);

		RenderTargetView = graphicsDevice->CreateRenderTargetView(TargetTexture);
		DepthStencilView = graphicsDevice->CreateDepthStencilBuffer(width, height);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;

		auto hr = device->CreateShaderResourceView(TargetTexture.Get(), &srvDesc, ShaderResourceView.GetAddressOf());

		if (FAILED(hr))
		{
			Log::WriteError("Error while creating SRV for render target");
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
