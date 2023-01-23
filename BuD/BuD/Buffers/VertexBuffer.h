#pragma once

#include "Buffer.h"

#include <vector>

namespace BuD
{
	class VertexBuffer : public Buffer
	{
	public:
		explicit VertexBuffer(size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const void* data = nullptr);

		inline size_t Stride() const { return m_Stride; }
		inline size_t Offset() const { return m_Offset; }

	protected:
		virtual BufferDesc GetBufferDesc(size_t byteWidth) const override;

		size_t m_Stride;
		size_t m_Offset;
	};
}
