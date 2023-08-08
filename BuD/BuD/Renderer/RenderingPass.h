#pragma once

#include <functional>
#include <optional>
#include <memory>

#include <Objects/MeshDetails.h>
#include <Renderer/ShaderPipeline.h>
#include <Renderer/InstanceData.h>
#include <Renderer/RasterizerDescription.h>

namespace BuD
{
	struct RenderingPass
	{
	public:
		std::function<void(const RenderingPass& renderPass, const Scene& scene)> m_PreRenderCallback = [](const RenderingPass& renderPass, const Scene& scene) {};

		MeshDetails m_Mesh;
		ShaderPipeline m_Pipeline;
		RasterizerDescription m_RasterizerDescription;

		std::optional<InstanceComponent> m_Instancing;

	public:
		bool operator==(const RenderingPass& other) const;
		bool operator!=(const RenderingPass& other) const;
		bool operator<=(const RenderingPass& other) const;
		bool operator>=(const RenderingPass& other) const;
		bool operator<(const RenderingPass& other) const;
		bool operator>(const RenderingPass& other) const;
	};
}
