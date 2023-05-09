#include "bud_pch.h"
#include "Texture.h"

#include <Renderer/Renderer.h>
#include <Renderer/Structures/Texture2DDesc.h>

namespace BuD
{
	Texture::Texture()
		: m_SRV(nullptr)
	{
	}
	
	Texture::Texture(ComPtr<ID3D11ShaderResourceView> texture)
		: m_SRV(texture)
	{
	}

	Texture Texture::LoadFromFile(std::filesystem::path filepath)
	{
		if (!std::filesystem::is_regular_file(filepath))
		{
			Log::WriteError(L"Attempting to load a resource which does not exist.");
			return {};
		}

		auto graphicsDevice = Renderer::Device();
		auto srv = graphicsDevice->CreateShaderResourceView(filepath);

		return Texture(srv);
	}
}
