#pragma once

#include "Shader.h"

#include <Renderer/Renderer.h>
#include <Utils/Log.h>

namespace BuD
{
	class VertexShader : public Shader
	{
	public:
		explicit VertexShader(void* code, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
		{
			auto graphicsDevice = Renderer::Device();
			auto& device = graphicsDevice->Device();

			if (FAILED(device->CreateVertexShader(code, size, nullptr, m_Shader.GetAddressOf())))
			{
				Log::WriteError(L"Error while creating vertex shader");
			}

			if (FAILED(device->CreateInputLayout(inputDesc.data(), static_cast<UINT>(inputDesc.size()), code, size, m_Layout.GetAddressOf())))
			{
				Log::WriteError(L"Error while creating vertex shader input layout");
			}
		}

		inline ID3D11VertexShader* Shader() { return m_Shader.Get(); }
		inline ID3D11InputLayout* GetLayout() { return m_Layout.Get(); }

	private:
		ComPtr<ID3D11InputLayout> m_Layout;
		ComPtr<ID3D11VertexShader> m_Shader;
	};
}
