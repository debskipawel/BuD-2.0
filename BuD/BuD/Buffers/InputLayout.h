#pragma once

#include <vector>

#include <d3d11.h>
#include <wrl/client.h>

#include <Renderer/GraphicsDevice.h>
#include <Renderer/Structures/BufferDesc.h>

using namespace Microsoft::WRL;

namespace BuD
{
	class InputLayout
	{
	public:
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout);

	private:
		ComPtr<ID3D11InputLayout> m_Layout;
		
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_LayoutElements;
		size_t m_LayoutAccumulatedSize;
	};
}
