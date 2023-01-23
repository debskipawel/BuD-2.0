#include "bud_pch.h"
#include "Shader.h"

namespace BuD
{
    std::vector<ID3D11Buffer*> Shader::RawConstantBuffers()
    {
        std::vector<ID3D11Buffer*> result(m_ConstantBuffers.size());

        std::transform(
            m_ConstantBuffers.begin(), m_ConstantBuffers.end(),
            result.begin(),
            [](std::shared_ptr<ConstantBuffer> buffer) { return buffer->Get(); }
        );

        return result;
    }

    std::vector<ID3D11ShaderResourceView*> Shader::RawTextures()
    {
        std::vector<ID3D11ShaderResourceView*> result(m_Textures.size());

        std::transform(
            m_Textures.begin(), m_Textures.end(),
            result.begin(),
            [](ComPtr<ID3D11ShaderResourceView> b) { return b.Get(); }
        );

        return result;
    }
}
