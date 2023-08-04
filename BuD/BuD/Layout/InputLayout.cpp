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
		std::map<DXGI_FORMAT, std::string> hlslTypeMap =
		{
			{ DXGI_FORMAT_R32G32B32A32_TYPELESS, "" },
			{ DXGI_FORMAT_R32G32B32A32_FLOAT, "float4" },
			{ DXGI_FORMAT_R32G32B32A32_UINT, "uint4" },
			{ DXGI_FORMAT_R32G32B32A32_SINT, "int4" },
			{ DXGI_FORMAT_R32G32B32_TYPELESS, "" },
			{ DXGI_FORMAT_R32G32B32_FLOAT, "float3" },
			{ DXGI_FORMAT_R32G32B32_UINT, "uint3" },
			{ DXGI_FORMAT_R32G32B32_SINT, "int3" },
			{ DXGI_FORMAT_R16G16B16A16_TYPELESS, "" },
			{ DXGI_FORMAT_R16G16B16A16_FLOAT, "half4" },
			{ DXGI_FORMAT_R32G32_TYPELESS, "" },
			{ DXGI_FORMAT_R32G32_FLOAT, "float2" },
			{ DXGI_FORMAT_R32G32_UINT, "uint2" },
			{ DXGI_FORMAT_R32G32_SINT, "int2" },
			{ DXGI_FORMAT_D32_FLOAT, "float" },
			{ DXGI_FORMAT_R32_FLOAT, "float" },
			{ DXGI_FORMAT_R32_UINT, "uint" },
			{ DXGI_FORMAT_R32_SINT, "int" },
		};

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
			[&argumentCount, &hlslTypeMap](std::string s, D3D11_INPUT_ELEMENT_DESC desc)
			{
				auto res = hlslTypeMap.find(desc.Format);

				std::string type = res == hlslTypeMap.end()
					? std::format("float{}", HelperFunctions::BytesPerFormat(desc.Format) / 4)
					: res->second;

				auto byteSize = HelperFunctions::BytesPerFormat(desc.Format);
				std::string argument = type + " arg" + std::to_string(argumentCount++) + " : " + desc.SemanticName + "; ";

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
