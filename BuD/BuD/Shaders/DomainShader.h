#pragma once

#include "Shader.h"

#include <Renderer/GraphicsDevice.h>
#include <Utils/Log.h>

namespace BuD
{
	class DomainShader : public Shader
	{
	public:
		explicit DomainShader(std::shared_ptr<GraphicsDevice> graphicsDevice, void* code, size_t size)
		{
			auto& device = graphicsDevice->Device();

			if (FAILED(device->CreateDomainShader(code, size, nullptr, m_Shader.GetAddressOf())))
			{
				Log::WriteError("Error while creating hull shader");
			}
		}

		inline ID3D11DomainShader* Shader() { return m_Shader.Get(); }
	private:
		ComPtr<ID3D11DomainShader> m_Shader;
	};
}
