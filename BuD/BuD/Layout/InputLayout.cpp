#include "bud_pch.h"
#include "InputLayout.h"

#include <Renderer/Renderer.h>
#include <Utils/HelperFunctions.h>

#include <numeric>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace BuD
{
	InputLayout::InputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout)
		: m_LayoutDescription(layout)
	{
		auto graphicsDevice = Renderer::Device();
		auto& device = graphicsDevice->Device();

		// input layout needs a compiled shader code to be created, but if shaders are completely separate, it needs to be simulated somehow
		// we can do it creating a shader for a specific layout, compiling it in runtime and destroying it afterwards 
		// stupid and ugly af but only happens once for every layout in the system, so not a big problem I guess
		int argumentCount = 0;
		std::string prefixFakeVS = "struct Vertex { ";
		std::string suffixFakeVS = "}; float4 main(Vertex i) : SV_Position { return float4(0.0, 0.0, 0.0, 1.0); }";
		
		auto vertexDefinition = std::accumulate(
			layout.begin(), layout.end(), std::string(),
			[&argumentCount](std::string s, D3D11_INPUT_ELEMENT_DESC desc)
			{
				auto byteSize = BytesPerFormat(desc.Format);
				std::string argument = "float" + std::to_string(byteSize / 4) + " arg" + std::to_string(argumentCount++) + " : " + desc.SemanticName + "; ";

				return s + argument;
			}
		);

		auto fakeShader = prefixFakeVS + vertexDefinition + suffixFakeVS;

		ComPtr<ID3DBlob> compiledShaderCode;
		ComPtr<ID3DBlob> compilerErrors;
		auto hr = D3DCompile(fakeShader.c_str(), fakeShader.size(), NULL, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0_level_9_1", 0, 0, compiledShaderCode.GetAddressOf(), compilerErrors.GetAddressOf());

		auto inputLayoutRes = device->CreateInputLayout(
			layout.data(), static_cast<UINT>(layout.size()),
			compiledShaderCode->GetBufferPointer(), compiledShaderCode->GetBufferSize(), 
			m_Layout.GetAddressOf()
		);

		if (FAILED(inputLayoutRes))
		{
			Log::WriteError(L"Error while creating input layout");
		}
	}

	bool InputLayout::operator==(const InputLayout& other) const
	{
		return m_Layout == other.m_Layout && m_LayoutDescription == other.m_LayoutDescription;
	}

	bool InputLayout::operator!=(const InputLayout& other) const
	{
		return !(*this == other);
	}
}
