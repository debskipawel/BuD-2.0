#pragma once

#include <d3d11.h>
#include <vector>

namespace BuD
{
	struct InputLayoutDescription
	{
	public:
		explicit InputLayoutDescription(std::vector<D3D11_INPUT_ELEMENT_DESC> layoutElements);
		explicit InputLayoutDescription(std::initializer_list<D3D11_INPUT_ELEMENT_DESC> layoutElements);

		bool operator==(const InputLayoutDescription& other) const;
		bool operator!=(const InputLayoutDescription& other) const;

		bool operator<(const InputLayoutDescription& other) const;
		bool operator>(const InputLayoutDescription& other) const;
		bool operator<=(const InputLayoutDescription& other) const;
		bool operator>=(const InputLayoutDescription& other) const;

	private:
		void CalculateAccumulatedByteSize();

		int CompareInputElements(const D3D11_INPUT_ELEMENT_DESC& left, const D3D11_INPUT_ELEMENT_DESC& right) const;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_LayoutElements;
		size_t m_AccumulatedByteSize;
	};
}
