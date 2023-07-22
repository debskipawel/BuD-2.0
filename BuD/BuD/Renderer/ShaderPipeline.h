#pragma once

#include <memory>

#include <Shaders/VertexShader.h>
#include <Shaders/HullShader.h>
#include <Shaders/DomainShader.h>
#include <Shaders/GeometryShader.h>
#include <Shaders/PixelShader.h>

namespace BuD
{
	struct ShaderPipeline
	{
	public:
		std::shared_ptr<VertexShader> m_VertexShader;
		std::shared_ptr<HullShader> m_HullShader;
		std::shared_ptr<DomainShader> m_DomainShader;
		std::shared_ptr<GeometryShader> m_GeometryShader;
		std::shared_ptr<PixelShader> m_PixelShader;

	public:
		bool operator==(const ShaderPipeline& other) const;
		bool operator!=(const ShaderPipeline& other) const;
	};
}
