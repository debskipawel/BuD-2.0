#pragma once

#include <vector>
#include <memory>
#include <functional>

#include <Buffers/ConstantBuffer.h>
#include <Scene/SceneEntity.h>

namespace BuD
{
	class Shader
	{
	public:
		inline void AddConstantBuffer(std::shared_ptr<ConstantBuffer> constantBuffer)
		{
			m_ConstantBuffers.push_back(constantBuffer);
		}

		inline void UpdateConstantBuffer(UINT bufferIndex, const void* data, size_t size)
		{
			m_ConstantBuffers[bufferIndex]->Update(data, size);
		}
		
		// TODO: make a Texture class
		inline void AddTexture(ComPtr<ID3D11ShaderResourceView> srv)
		{
			m_Textures.push_back(srv);
		}
		
		inline void UpdateTexture(UINT textureIndex, ComPtr<ID3D11ShaderResourceView> srv)
		{
			m_Textures[textureIndex] = srv;
		}

		inline const auto& ConstantBuffers() const { return m_ConstantBuffers; }
		inline const auto& Textures() const { return m_Textures; }
		
		std::vector<ID3D11Buffer*> RawConstantBuffers();
		std::vector<ID3D11ShaderResourceView*> RawTextures();

	protected:
		explicit Shader() = default;

		std::vector<std::shared_ptr<ConstantBuffer>> m_ConstantBuffers;
		std::vector<ComPtr<ID3D11ShaderResourceView>> m_Textures;
	};
}
