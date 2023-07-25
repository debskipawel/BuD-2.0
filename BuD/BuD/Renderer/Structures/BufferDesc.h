#pragma once

#include <d3d11.h>

namespace BuD
{
	class BufferDesc : public D3D11_BUFFER_DESC
	{
	public:
		BufferDesc(UINT bindFlags, size_t byteWidth)
		{
			ZeroMemory(this, sizeof(BufferDesc));
			BindFlags = bindFlags;
			ByteWidth = static_cast<UINT>(byteWidth);
			Usage = D3D11_USAGE_DEFAULT;
		}

		inline static BufferDesc VertexBufferDescription(size_t byteWidth)
		{
			return { D3D11_BIND_VERTEX_BUFFER, byteWidth };
		}

		inline static BufferDesc IndexBufferDescription(size_t byteWidth)
		{
			return { D3D11_BIND_INDEX_BUFFER, byteWidth };
		}

		inline static BufferDesc ConstantBufferDescription(size_t byteWidth)
		{
			BufferDesc desc{ D3D11_BIND_CONSTANT_BUFFER, byteWidth };
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			return desc;
		}
	};
}
