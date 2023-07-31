#include "bud_pch.h"
#include "ShaderPipeline.h"

namespace BuD
{
	bool ShaderPipeline::operator==(const ShaderPipeline& other) const
	{
		return m_VertexShader == other.m_VertexShader
			&& m_PixelShader == other.m_PixelShader
			&& m_GeometryShader == other.m_GeometryShader
			&& m_HullShader == other.m_HullShader
			&& m_DomainShader == other.m_DomainShader;
	}

	bool BuD::ShaderPipeline::operator!=(const ShaderPipeline& other) const
	{
		return !(*this == other);
	}
	
	bool ShaderPipeline::operator<=(const ShaderPipeline& other) const
	{
		return !(*this > other);
	}
	
	bool ShaderPipeline::operator>=(const ShaderPipeline& other) const
	{
		return !(*this < other);
	}
	
	bool ShaderPipeline::operator<(const ShaderPipeline& other) const
	{
		return m_VertexShader < other.m_VertexShader
			|| (m_VertexShader == other.m_VertexShader 
				&& m_PixelShader < other.m_PixelShader)
			|| (m_VertexShader == other.m_VertexShader 
				&& m_PixelShader == other.m_PixelShader 
				&& m_GeometryShader < other.m_GeometryShader)
			|| (m_VertexShader == other.m_VertexShader
				&& m_PixelShader == other.m_PixelShader
				&& m_GeometryShader == other.m_GeometryShader 
				&& m_HullShader < other.m_HullShader)
			|| (m_VertexShader == other.m_VertexShader
				&& m_PixelShader == other.m_PixelShader
				&& m_GeometryShader == other.m_GeometryShader
				&& m_HullShader == other.m_HullShader 
				&& m_DomainShader < other.m_DomainShader);
	}
	
	bool ShaderPipeline::operator>(const ShaderPipeline& other) const
	{
		return m_VertexShader > other.m_VertexShader
			|| (m_VertexShader == other.m_VertexShader
				&& m_PixelShader > other.m_PixelShader)
			|| (m_VertexShader == other.m_VertexShader
				&& m_PixelShader == other.m_PixelShader
				&& m_GeometryShader > other.m_GeometryShader)
			|| (m_VertexShader == other.m_VertexShader
				&& m_PixelShader == other.m_PixelShader
				&& m_GeometryShader == other.m_GeometryShader
				&& m_HullShader > other.m_HullShader)
			|| (m_VertexShader == other.m_VertexShader
				&& m_PixelShader == other.m_PixelShader
				&& m_GeometryShader == other.m_GeometryShader
				&& m_HullShader == other.m_HullShader
				&& m_DomainShader > other.m_DomainShader);
	}
}
