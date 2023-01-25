#pragma once

#include <d3d11.h>
#include <filesystem>
#include <wrl/client.h>

#include "GraphicsDevice.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(ComPtr<ID3D11ShaderResourceView> texture);

		inline ID3D11ShaderResourceView* SRV() const { return m_SRV.Get(); }

		static Texture LoadFromFile(std::filesystem::path filepath);

	protected:
		ComPtr<ID3D11ShaderResourceView> m_SRV;
	};
}
