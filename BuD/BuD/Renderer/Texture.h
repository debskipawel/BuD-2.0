#pragma once

#include <d3d11.h>
#include <filesystem>
#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace BuD
{
	class Texture
	{
	public:
		Texture();
		Texture(const Texture& other) = default;
		Texture(Texture&& other) = default;
		Texture& operator=(const Texture& other) = default;

		Texture(ComPtr<ID3D11ShaderResourceView> texture);

		inline ID3D11ShaderResourceView* SRV() const { return m_SRV.Get(); }

		static Texture LoadFromFile(std::filesystem::path filepath);

	protected:
		ComPtr<ID3D11ShaderResourceView> m_SRV;
	};
}
