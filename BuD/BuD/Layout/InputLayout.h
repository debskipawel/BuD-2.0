#pragma once

#include <vector>

#include <d3d11.h>
#include <wrl/client.h>

#include <Layout/InputLayoutDescription.h>

using namespace Microsoft::WRL;

namespace BuD
{
	class InputLayout
	{
	public:
		InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout);

		ID3D11InputLayout* Layout() const { return m_Layout.Get(); }

		bool operator==(const InputLayout& other) const;
		bool operator!=(const InputLayout& other) const;

		const auto& GetDescription() const { return m_LayoutDescription; }

	private:
		ComPtr<ID3D11InputLayout> m_Layout;
		
		InputLayoutDescription m_LayoutDescription;
	};
}
