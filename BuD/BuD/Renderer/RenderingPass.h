#pragma once

#include <functional>
#include <memory>

#include <Buffers/VertexBuffer.h>
#include <Buffers/IndexBuffer.h>

#include <Shaders/VertexShader.h>
#include <Shaders/HullShader.h>
#include <Shaders/DomainShader.h>
#include <Shaders/GeometryShader.h>
#include <Shaders/PixelShader.h>

#include <Scene/SceneEntity.h>

namespace BuD
{
	struct RenderingPass
	{
		std::function<void(const RenderingPass& renderPass, SceneEntity entity)> PreRenderCallback = [](const RenderingPass& renderPass, SceneEntity entity) {};

		std::shared_ptr<VertexBuffer>	VertexBuffer;
		std::shared_ptr<IndexBuffer>	IndexBuffer;

		std::shared_ptr<VertexShader>	VertexShader;
		std::shared_ptr<HullShader>		HullShader;
		std::shared_ptr<DomainShader>	DomainShader;
		std::shared_ptr<GeometryShader> GeometryShader;
		std::shared_ptr<PixelShader>	PixelShader;
	};
}
