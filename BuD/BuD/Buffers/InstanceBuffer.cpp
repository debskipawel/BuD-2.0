#include "bud_pch.h"
#include "InstanceBuffer.h"

namespace BuD
{
	BufferDesc InstanceBuffer::GetBufferDesc(size_t byteWidth) const
	{
		auto res = BufferDesc{ D3D11_BIND_VERTEX_BUFFER, byteWidth };

		res.Usage = D3D11_USAGE_DYNAMIC;
		res.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		return res;
	}
}
