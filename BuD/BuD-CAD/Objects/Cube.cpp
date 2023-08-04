#include "Cube.h"

Cube::Cube(BuD::Scene& scene)
	: SceneObjectCAD(scene)
{
	m_Tag = "Cube";

	BuD::MeshLoader meshLoader;
	auto mesh = meshLoader.LoadPrimitiveMesh(BuD::MeshPrimitiveType::CUBE);

	BuD::ShaderPipeline pipeline;
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/Cube/cube_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/Cube/cube_ps.hlsl");

	BuD::RenderingPass renderingPass;
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = pipeline;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto vs = renderingPass.m_Pipeline.m_VertexShader;
		auto camera = scene.ActiveCamera();

		dxm::Matrix cb[] = { camera->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };
		vs->UpdateConstantBuffer(0, cb, 2 * sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderPasses;
	renderPasses.push_back(renderingPass);

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderPasses);
}
