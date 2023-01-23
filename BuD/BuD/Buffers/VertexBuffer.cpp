#include "bud_pch.h"
#include "VertexBuffer.h"

namespace BuD
{
	VertexBuffer::VertexBuffer(size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const void* data)
		: Buffer(data, GetBufferDesc(size)), m_Offset(0)
	{
		auto strideBit = std::accumulate(
			layout.begin(), layout.end(), (size_t)0, 
			[](size_t a, D3D11_INPUT_ELEMENT_DESC desc) { return a + BitsPerPixel(desc.Format); }
		);

		m_Stride = strideBit / 8;
	}

	BufferDesc VertexBuffer::GetBufferDesc(size_t byteWidth) const
	{
		auto res = BufferDesc{ D3D11_BIND_VERTEX_BUFFER, byteWidth };

		res.Usage = D3D11_USAGE_DYNAMIC;
		res.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		return res;
	}
}
