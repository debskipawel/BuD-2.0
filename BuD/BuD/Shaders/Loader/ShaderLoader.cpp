#include "bud_pch.h"
#include "ShaderLoader.h"

#include <fstream>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

std::vector<BYTE> LoadByteCode(const std::wstring& shaderPath)
{
	std::ifstream sIn(shaderPath, std::ios::in | std::ios::binary);

	sIn.seekg(0, std::ios::end);
	auto byteCodeLength = sIn.tellg();
	sIn.seekg(0, std::ios::beg);

	std::vector<BYTE> byteCode(byteCodeLength);

	sIn.read(reinterpret_cast<char*>(byteCode.data()), byteCodeLength);
	sIn.close();

	return byteCode;
}

bool SaveToFile(const void* data, size_t size, const std::wstring& path)
{
	try
	{
		std::ofstream sOut(path, std::ios::out | std::ios::binary);
		sOut.write(reinterpret_cast<const char*>(data), size);
		sOut.close();

		return true;
	}
	catch (std::exception)
	{
		return false;
	}
}

HRESULT CompileShader(
	_In_ LPCWSTR srcFile,
	_In_ LPCSTR entryPoint,
	_In_ LPCSTR profile,
	_Outptr_ ID3DBlob** blob
)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
		{
			shaderBlob->Release();
		}

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

namespace BuD
{
	template<typename T>
	std::shared_ptr<T> ShaderLoad(
		std::map<std::filesystem::path, std::shared_ptr<T>>& map,
		std::filesystem::path shaderPath,
		const std::vector<size_t>& constants,
		std::string mainFunName,
		std::string shaderVersion
	)
	{
		auto result = map.find(shaderPath);

		if (result != map.end())
		{
			return result->second;
		}

		std::shared_ptr<T> shader;

		if (shaderPath.extension() == L".cso")
		{
			auto bytecode = LoadByteCode(shaderPath);
			shader = std::make_shared<T>(bytecode.data(), bytecode.size());
		}
		else
		{
			std::filesystem::path compiledShaderName;
			compiledShaderName += shaderPath.root_path();
			compiledShaderName += shaderPath.relative_path();
			compiledShaderName += shaderPath.filename();
			compiledShaderName += L".cso";

			try
			{
				// try loading compiled shader if exists
				auto bytecode = LoadByteCode(compiledShaderName);
				shader = std::make_shared<T>(bytecode.data(), bytecode.size());
			}
			catch (std::exception e)
			{
				ID3DBlob* blob = nullptr;
				auto hr = CompileShader(shaderPath.wstring().c_str(), mainFunName.c_str(), shaderVersion.c_str(), &blob);

				shader = std::make_shared<T>(blob->GetBufferPointer(), blob->GetBufferSize());

				SaveToFile(blob->GetBufferPointer(), blob->GetBufferSize(), compiledShaderName);

				blob->Release();
			}
		}

		for (auto& size : constants)
		{
			auto cb = std::make_shared<ConstantBuffer>(size);
			shader->AddConstantBuffer(cb);
		}

		map.insert(std::make_pair(shaderPath, shader));

		return shader;
	}

	void ShaderLoader::Clear()
	{
		s_VertexShaders.clear();
		s_PixelShaders.clear();
		s_DomainShaders.clear();
		s_HullShaders.clear();
		s_GeometryShaders.clear();
	}

	std::shared_ptr<VertexShader> ShaderLoader::VSLoad(std::filesystem::path shaderPath, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const std::vector<size_t>& constants, std::string mainFunName)
	{
		auto result = s_VertexShaders.find(shaderPath);

		if (result != s_VertexShaders.end())
		{
			return result->second;
		}

		std::shared_ptr<VertexShader> shader;

		if (shaderPath.extension() == L".cso")
		{
			auto bytecode = LoadByteCode(shaderPath);
			shader = std::make_shared<VertexShader>(bytecode.data(), bytecode.size(), layout);
		}
		else
		{
			std::filesystem::path compiledShaderName; 
			compiledShaderName += shaderPath.root_path();
			compiledShaderName += shaderPath.relative_path();
			compiledShaderName += shaderPath.filename();
			compiledShaderName += L".cso";

			try
			{
				auto bytecode = LoadByteCode(compiledShaderName);
				shader = std::make_shared<VertexShader>(bytecode.data(), bytecode.size(), layout);
			}
			catch (std::exception e)
			{
				ID3DBlob* vsBlob = nullptr;
				auto hr = CompileShader(shaderPath.c_str(), mainFunName.c_str(), "vs_4_0_level_9_1", &vsBlob);
				shader = std::make_shared<VertexShader>(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), layout);
				vsBlob->Release();

				SaveToFile(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), compiledShaderName);
			}
		}

		for (auto& size : constants)
		{
			auto cb = std::make_shared<ConstantBuffer>(size);
			shader->AddConstantBuffer(cb);
		}

		s_VertexShaders.insert(std::make_pair(shaderPath, shader));

		return shader;
	}

	std::shared_ptr<HullShader> ShaderLoader::HSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_HullShaders, shaderPath, constants, mainFunName, "hs_4_0_level_9_1");
	}

	std::shared_ptr<DomainShader> ShaderLoader::DSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_DomainShaders, shaderPath, constants, mainFunName, "ds_4_0_level_9_1");
	}

	std::shared_ptr<GeometryShader> ShaderLoader::GSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_GeometryShaders, shaderPath, constants, mainFunName, "gs_4_0_level_9_1");
	}

	std::shared_ptr<PixelShader> ShaderLoader::PSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_PixelShaders, shaderPath, constants, mainFunName, "ps_4_0_level_9_1");
	}
}
