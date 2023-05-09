#pragma once

#include <d3d11.h>
#include <memory>
#include <wrl/client.h>

#include "GraphicsDevice.h"

using namespace Microsoft::WRL;

namespace BuD
{
	struct RenderTarget
	{
		RenderTarget(std::shared_ptr<GraphicsDevice> device, int width, int height);
		~RenderTarget();

		ComPtr<ID3D11Texture2D> TargetTexture;
		ComPtr<ID3D11RenderTargetView> RenderTargetView;
		ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
		ComPtr<ID3D11DepthStencilView> DepthStencilView;

		UINT Width, Height;
		int FramesWithoutUsage;
	};

	struct RenderTargetInfo
	{
		ComPtr<ID3D11RenderTargetView> RenderTargetView;
		ComPtr<ID3D11DepthStencilView> DepthStencilView;

		UINT Width, Height;
	};
}
