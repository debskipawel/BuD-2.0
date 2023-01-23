#include "bud_pch.h"
#include "IndexBuffer.h"

namespace BuD
{
	IndexBuffer::IndexBuffer(DXGI_FORMAT format, size_t size, const void* data, D3D_PRIMITIVE_TOPOLOGY topology)
		: Buffer(data, GetBufferDesc(size)), m_Format(format), m_Topology(topology)
	{
		auto bpp = BitsPerPixel(format);
		m_IndicesCount = size / (bpp / 8);
	}

	void IndexBuffer::Update(const void* data, size_t size)
	{
		Buffer::Update(data, size);

		m_IndicesCount = size / (BitsPerPixel(m_Format) / 8);
	}

	BufferDesc IndexBuffer::GetBufferDesc(size_t byteWidth) const
	{
		auto res = BufferDesc{ D3D11_BIND_INDEX_BUFFER, byteWidth };

		res.Usage = D3D11_USAGE_DYNAMIC;
		res.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		return res;
	}
}
