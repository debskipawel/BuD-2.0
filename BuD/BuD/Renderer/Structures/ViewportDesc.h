#pragma once

#include <d3d11.h>

namespace BuD
{
	struct ViewportDesc : D3D11_VIEWPORT
	{
		explicit ViewportDesc(UINT width, UINT height)
		{
			TopLeftX = 0.0f;
			TopLeftY = 0.0f;
			Width = static_cast<FLOAT>(width);
			Height = static_cast<FLOAT>(height);
			MinDepth = 0.0f;
			MaxDepth = 1.0f;
		}
	};
}
