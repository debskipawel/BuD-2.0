#include "Grid.h"

Grid::Grid(BuD::Scene& scene)
{
	m_GridEntity = BuD::SceneEntity(scene);

	auto meshLoader = BuD::MeshLoader();
	auto mesh = meshLoader.LoadPrimitiveMesh(BuD::MeshPrimitiveType::QUAD);

	BuD::ShaderPipeline pipeline = {};
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/Grid/grid_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/Grid/grid_ps.hlsl");

	BuD::RenderingPass renderingPass = {};
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = pipeline;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();

		auto invViewMtx = camera->ViewMatrix();
		invViewMtx.Invert();

		auto invProjMtx = BuD::Renderer::ProjectionMatrix();
		invProjMtx.Invert();

		dxm::Matrix vsCB[] = { invViewMtx, invProjMtx };

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses;
	renderingPasses.push_back(renderingPass);

	m_GridEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}
