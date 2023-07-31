#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include <Renderer/GraphicsDevice.h>
#include <Renderer/Structures/BufferDesc.h>

using namespace Microsoft::WRL;

namespace BuD
{
	class Buffer
	{
	public:
		Buffer(const void* data, BufferDesc desc);
		virtual ~Buffer() = default;

		inline ID3D11Buffer* Get() const { return m_Buffer.Get(); }

		virtual void Update(const void* data, size_t size);

	protected:
		virtual BufferDesc GetBufferDesc(size_t byteWidth) const = 0;

		virtual void CreateBuffer(const void* data, const BufferDesc& desc);

		BufferDesc m_Description;
		ComPtr<ID3D11Buffer> m_Buffer;
	};
}
