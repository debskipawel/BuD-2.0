#include "bud_pch.h"
#include "PointLight.h"

#include <Resources/Meshes/Cube.h>
#include <Shaders/Loader/ShaderLoader.h>

namespace BuD
{
	PointLight::PointLight(Scene& scene, dxm::Vector3 position, dxm::Vector3 color)
	{
		m_Entity = SceneEntity(scene);

		auto& state = m_Entity.AddComponent<IState>();
		state.Position = position;

		auto& emissive = m_Entity.AddComponent<IEmissive>();
		emissive.Color = color;

		auto& positions = g_cubePositionVertices;
		auto& indices = g_cubePositionIndices;

		auto renderPass = RenderingPass{};

		renderPass.VertexBuffer = std::make_shared<VertexBuffer>(positions.size() * sizeof(Vector3), g_cubePositionLayout, positions.data());
		renderPass.IndexBuffer = std::make_shared<IndexBuffer>(DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data());

		renderPass.VertexShader = ShaderLoader::VSLoad(L"../x64/Debug/mvp_pos_vs.hlsl", g_cubePositionLayout, { 4 * sizeof(Matrix) });
		renderPass.PixelShader = ShaderLoader::PSLoad(L"../x64/Debug/solidColor_ps.hlsl", { sizeof(dxm::Vector3) });

		renderPass.PreRenderCallback = [this, &emissive](const RenderingPass& pass, SceneEntity entity)
		{
			auto& state = entity.GetComponent<IState>();
			auto& vs = pass.VertexShader;
			auto& ps = pass.PixelShader;

			auto model = dxm::Matrix::CreateTranslation(state.Position);
			auto view = entity->ActiveCamera()->ViewMatrix();
			auto proj = Renderer::ProjectionMatrix();

			dxm::Matrix cb[] =
			{
				model,
				view,
				view.Invert(),
				proj
			};

			vs->UpdateConstantBuffer(0, cb, 4 * sizeof(Matrix));
			ps->UpdateConstantBuffer(0, &emissive.Color, sizeof(dxm::Vector3));
		};

		m_Entity.AddComponent<IRenderable>(std::vector<RenderingPass>{ renderPass });
	}
}
