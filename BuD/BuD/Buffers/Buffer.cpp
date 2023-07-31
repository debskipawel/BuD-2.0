#include "bud_pch.h"
#include "Buffer.h"

#include <Renderer/Renderer.h>

namespace BuD
{
    Buffer::Buffer(const void* data, BufferDesc desc)
        : m_Description(desc)
    {
        CreateBuffer(data, m_Description);
    }

    void Buffer::Update(const void* data, size_t size)
    {
        if (size <= m_Description.ByteWidth)
        {
            auto graphicsDevice = Renderer::Device();
            auto& context = graphicsDevice->Context();

            D3D11_MAPPED_SUBRESOURCE res;

            if (data)
            {
                auto hr = context->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
                memcpy(res.pData, data, size);
                context->Unmap(m_Buffer.Get(), 0);
            }

            return;
        }

        m_Description.ByteWidth = size;

        m_Buffer.Reset();

        CreateBuffer(data, m_Description);
    }

    void Buffer::CreateBuffer(const void* data, const BufferDesc& desc)
    {
        D3D11_SUBRESOURCE_DATA sdata;
        ZeroMemory(&sdata, sizeof sdata);
        sdata.pSysMem = data;

        auto bufferDesc = desc;
        if (bufferDesc.ByteWidth % 16 != 0)
        {
            bufferDesc.ByteWidth = (bufferDesc.ByteWidth / 16 + 1) * 16;
        }

        auto graphicsDevice = Renderer::Device();
        auto& device = graphicsDevice->Device();

        ID3D11Buffer* temp;
        if (FAILED(device->CreateBuffer(&bufferDesc, data ? &sdata : nullptr, &temp)))
        {
            Log::WriteError(L"Error while creating a buffer.");
        }

        m_Buffer.Attach(temp);
    }
}