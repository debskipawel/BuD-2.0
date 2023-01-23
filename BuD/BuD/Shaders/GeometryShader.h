#pragma once

#include "Shader.h"

#include <Renderer/Renderer.h>
#include <Utils/Log.h>

namespace BuD
{
	class GeometryShader : public Shader
	{
	public:
		explicit GeometryShader(void* code, size_t size)
		{
			auto graphicsDevice = Renderer::Device();
			auto& device = graphicsDevice->Device();

			if (FAILED(device->CreateGeometryShader(code, size, nullptr, m_Shader.GetAddressOf())))
			{
				Log::WriteError(L"Error while creating geometry shader");
			}
		}

		inline ID3D11GeometryShader* Shader() { return m_Shader.Get(); }
	private:
		ComPtr<ID3D11GeometryShader> m_Shader;
	};
}
