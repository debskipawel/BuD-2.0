#pragma once

#include "Shader.h"

#include <Renderer/GraphicsDevice.h>
#include <Utils/Log.h>

namespace BuD
{
	class PixelShader : public Shader
	{
	public:
		explicit PixelShader(std::shared_ptr<GraphicsDevice> graphicsDevice, void* code, size_t size)
		{
			auto& device = graphicsDevice->Device();

			if (FAILED(device->CreatePixelShader(code, size, nullptr, m_Shader.GetAddressOf())))
			{
				Log::WriteError(L"Error while creating pixel shader");
			}
		}

		inline ID3D11PixelShader* Shader() { return m_Shader.Get(); }
	private:
		ComPtr<ID3D11PixelShader> m_Shader;
	};
}
