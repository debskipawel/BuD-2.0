#pragma once

#include "Buffer.h"

#include <Renderer/Renderer.h>

namespace BuD
{
	template <typename T>
	class StructuredBuffer : public Buffer
	{
	public:
		StructuredBuffer() = default;

		virtual BufferDesc GetBufferDesc(size_t byteWidth) const override;

		virtual void CreateBuffer(const void* data, const BufferDesc& desc) override
		{
			Buffer::CreateBuffer(data, desc);

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.FirstElement = 0;
			srvDesc.Buffer.NumElements = desc.ByteWidth / sizeof(T);

			auto graphicsDevice = Renderer::Device();
			auto& device = graphicsDevice->Device();
			device->CreateShaderResourceView(m_Buffer.Get(), &srvDesc, m_SRV.GetAddressOf());
		}

	protected:
		ComPtr<ID3D11ShaderResourceView> m_SRV;
	};
}
