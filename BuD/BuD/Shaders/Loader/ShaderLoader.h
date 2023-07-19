#pragma once

#include <Shaders/VertexShader.h>
#include <Shaders/HullShader.h>
#include <Shaders/DomainShader.h>
#include <Shaders/GeometryShader.h>
#include <Shaders/PixelShader.h>

#include <filesystem>

namespace BuD
{
	class ShaderLoader
	{
	public:
		ShaderLoader() = delete;

		static void Clear();

		static std::shared_ptr<VertexShader> VSLoad(
			std::filesystem::path shaderPath,
			const std::vector<size_t>& constants = {},
			std::string mainFunName = "main"
		);

		static std::shared_ptr<HullShader> HSLoad(
			std::filesystem::path shaderPath,
			const std::vector<size_t>& constants = {},
			std::string mainFunName = "main"
		);

		static std::shared_ptr<DomainShader> DSLoad(
			std::filesystem::path shaderPath,
			const std::vector<size_t>& constants = {},
			std::string mainFunName = "main"
		);

		static std::shared_ptr<GeometryShader> GSLoad(
			std::filesystem::path shaderPath,
			const std::vector<size_t>& constants = {},
			std::string mainFunName = "main"
		);

		static std::shared_ptr<PixelShader> PSLoad(
			std::filesystem::path shaderPath,
			const std::vector<size_t>& constants = {},
			std::string mainFunName = "main"
		);

	protected:
		inline static std::map<std::filesystem::path, std::shared_ptr<VertexShader>> s_VertexShaders = {};
		inline static std::map<std::filesystem::path, std::shared_ptr<HullShader>> s_HullShaders = {};
		inline static std::map<std::filesystem::path, std::shared_ptr<DomainShader>> s_DomainShaders = {};
		inline static std::map<std::filesystem::path, std::shared_ptr<GeometryShader>> s_GeometryShaders = {};
		inline static std::map<std::filesystem::path, std::shared_ptr<PixelShader>> s_PixelShaders = {};
	};
}
