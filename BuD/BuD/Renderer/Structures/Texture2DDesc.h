#pragma once

#include <d3d11.h>

namespace BuD
{
	struct Texture2DDesc : D3D11_TEXTURE2D_DESC
	{
		Texture2DDesc(UINT width, UINT height)
		{
			ZeroMemory(this, sizeof(Texture2DDesc));
			Width = width;
			Height = height;
			//MipLevels = 0;
			ArraySize = 1;
			Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SampleDesc.Count = 1;
			SampleDesc.Quality = 0;
			Usage = D3D11_USAGE_DEFAULT;
			BindFlags = D3D11_BIND_SHADER_RESOURCE;
			//CPUAccessFlags = 0;
			//MiscFlags = 0;
		}

		static Texture2DDesc DepthStencilDescription(UINT width, UINT height)
		{
			auto res = Texture2DDesc(width, height);

			res.MipLevels = 1;
			res.Format = DXGI_FORMAT_D32_FLOAT;
			res.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			return res;
		}
	};
}
