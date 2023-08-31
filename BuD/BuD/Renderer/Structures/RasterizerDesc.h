#pragma once

#include <d3d11.h>

namespace BuD
{
	struct RasterizerDesc : public D3D11_RASTERIZER_DESC
	{
		explicit RasterizerDesc()
		{
			FrontCounterClockwise = false;
			DepthClipEnable = true;
			DepthBias = 0;
			DepthBiasClamp = 0.0f;
			SlopeScaledDepthBias = 0.0f;
			ScissorEnable = false;
			MultisampleEnable = false;
			AntialiasedLineEnable = false;
		}
	};
}
