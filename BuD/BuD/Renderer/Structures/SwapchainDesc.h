#pragma once

#include <d3d11.h>

namespace BuD
{
	struct SwapchainDesc : public DXGI_SWAP_CHAIN_DESC
	{
		SwapchainDesc(HWND wndHwnd, UINT width, UINT height)
		{
			ZeroMemory(this, sizeof(SwapchainDesc));
			BufferDesc.Width = width;
			BufferDesc.Height = height;
			BufferDesc.RefreshRate.Numerator = 0;
			BufferDesc.RefreshRate.Denominator = 1;
			BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			//BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //0
			BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
			SampleDesc.Quality = 0;
			SampleDesc.Count = 1;
			BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			BufferCount = 1;
			OutputWindow = wndHwnd;
			Windowed = true;
			//SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //0
			//Flags = 0;
		}
	};
}
