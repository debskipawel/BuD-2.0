#include "bud_pch.h"
#include "ShaderLoader.h"

#include <Renderer/Renderer.h>

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

		auto graphicsDevice = Renderer::Device();
		std::shared_ptr<T> shader;

		if (shaderPath.extension() == L".cso")
		{
			auto bytecode = LoadByteCode(shaderPath);
			shader = std::make_shared<T>(graphicsDevice, bytecode.data(), bytecode.size());
		}
		else
		{
			std::filesystem::path currentPath = std::filesystem::current_path();
			std::filesystem::path sourceShaderPath = currentPath / shaderPath;
			std::filesystem::path compiledShaderName = (currentPath / shaderPath).replace_extension(L".cso");

			//try
			//{
			//	// try loading compiled shader if exists
			//	auto bytecode = LoadByteCode(compiledShaderName);
			//	shader = std::make_shared<T>(graphicsDevice, bytecode.data(), bytecode.size());
			//}
			//catch (std::exception e)
			{
				ID3DBlob* blob = nullptr;
				auto hr = CompileShader(sourceShaderPath.c_str(), mainFunName.c_str(), shaderVersion.c_str(), &blob);

				shader = std::make_shared<T>(graphicsDevice, blob->GetBufferPointer(), blob->GetBufferSize());

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

	std::shared_ptr<VertexShader> ShaderLoader::VSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_VertexShaders, shaderPath, constants, mainFunName, "vs_5_0");
	}

	std::shared_ptr<HullShader> ShaderLoader::HSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_HullShaders, shaderPath, constants, mainFunName, "hs_5_0");
	}

	std::shared_ptr<DomainShader> ShaderLoader::DSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_DomainShaders, shaderPath, constants, mainFunName, "ds_5_0");
	}

	std::shared_ptr<GeometryShader> ShaderLoader::GSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_GeometryShaders, shaderPath, constants, mainFunName, "gs_5_0");
	}

	std::shared_ptr<PixelShader> ShaderLoader::PSLoad(std::filesystem::path shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(s_PixelShaders, shaderPath, constants, mainFunName, "ps_5_0");
	}
}
