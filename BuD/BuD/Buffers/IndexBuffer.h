#pragma once

#include "Buffer.h"

namespace BuD
{
	class IndexBuffer : public Buffer
	{
	public:
		explicit IndexBuffer(DXGI_FORMAT format, size_t size, const void* data = nullptr, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		inline size_t Count() const { return m_IndicesCount; }
		inline DXGI_FORMAT Format() const { return m_Format; }
		inline D3D_PRIMITIVE_TOPOLOGY Topology() { return m_Topology; }

		virtual void Update(const void* data, size_t size) override;

	protected:
		virtual BufferDesc GetBufferDesc(size_t byteWidth) const override;

		D3D_PRIMITIVE_TOPOLOGY m_Topology;
		DXGI_FORMAT m_Format;
		size_t m_IndicesCount;
	};
}
