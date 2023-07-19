#pragma once

#include "Shader.h"

#include <Renderer/Renderer.h>
#include <Utils/Log.h>

namespace BuD
{
	class VertexShader : public Shader
	{
	public:
		explicit VertexShader(void* code, size_t size)
		{
			auto graphicsDevice = Renderer::Device();
			auto& device = graphicsDevice->Device();

			if (FAILED(device->CreateVertexShader(code, size, nullptr, m_Shader.GetAddressOf())))
			{
				Log::WriteError(L"Error while creating vertex shader");
			}
		}

		inline ID3D11VertexShader* Shader() { return m_Shader.Get(); }

	private:
		ComPtr<ID3D11VertexShader> m_Shader;
	};
}
