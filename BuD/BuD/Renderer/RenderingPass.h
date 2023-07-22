#pragma once

#include <functional>
#include <memory>

#include <Objects/MeshDetails.h>
#include <Renderer/ShaderPipeline.h>

#include <Scene/SceneEntity.h>

namespace BuD
{
	struct RenderingPass
	{
	public:
		std::function<void(const RenderingPass& renderPass, SceneEntity entity)> m_PreRenderCallback = [](const RenderingPass& renderPass, SceneEntity entity) {};

		MeshDetails m_Mesh;
		ShaderPipeline m_Pipeline;

	public:
		bool operator==(const RenderingPass& other) const;
		bool operator!=(const RenderingPass& other) const;
	};
}
