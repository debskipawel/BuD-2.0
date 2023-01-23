#pragma once

#include "Shader.h"

#include <Renderer/Renderer.h>
#include <Utils/Log.h>

namespace BuD
{
	class HullShader : public Shader
	{
	public:
		explicit HullShader(void* code, size_t size)
		{
			auto graphicsDevice = Renderer::Device();
			auto& device = graphicsDevice->Device();

			if (FAILED(device->CreateHullShader(code, size, nullptr, m_Shader.GetAddressOf())))
			{
				Log::WriteError(L"Error while creating hull shader");
			}
		}

		inline ID3D11HullShader* Shader() { return m_Shader.Get(); }
	private:
		ComPtr<ID3D11HullShader> m_Shader;
	};
}
