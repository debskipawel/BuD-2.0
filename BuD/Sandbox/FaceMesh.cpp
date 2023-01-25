#include "FaceMesh.h"

#include <Objects/MeshLoader/MeshLoader.h>
#include <Shaders/Loader/ShaderLoader.h>

FaceMesh::FaceMesh(BuD::Scene& scene)
{
	m_Entity = BuD::SceneEntity(scene);

	BuD::MeshLoader loader;
	auto meshSegments = loader.LoadMesh("Resources/RogerHead.obj");

	BuD::RenderingPass renderPass{};

	renderPass.VertexBuffer = meshSegments[0].VertexBuffer;
	renderPass.IndexBuffer = meshSegments[0].IndexBuffer;

	renderPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/mvp_pos_vs.hlsl", meshSegments[0].Layout, { 4 * sizeof(dxm::Matrix) });
	renderPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/solid_white_ps.hlsl");

	renderPass.PreRenderCallback = [](const BuD::RenderingPass& pass, BuD::SceneEntity entity)
	{
		auto& vs = pass.VertexShader;
		auto& state = entity.GetComponent<BuD::IState>();

		auto model = dxm::Matrix::CreateFromQuaternion(state.Rotation) * dxm::Matrix::CreateTranslation(state.Position);
		auto view = entity->ActiveCamera()->ViewMatrix();
		auto proj = BuD::Renderer::ProjectionMatrix();

		dxm::Matrix cb[] =
		{
			model,
			view,
			view.Invert(),
			proj
		};

		vs->UpdateConstantBuffer(0, cb, 4 * sizeof(dxm::Matrix));
	};

	m_Entity.AddComponent<BuD::IRenderable>(std::vector<BuD::RenderingPass>{ renderPass });
	m_Entity.AddComponent<BuD::IState>();
}
