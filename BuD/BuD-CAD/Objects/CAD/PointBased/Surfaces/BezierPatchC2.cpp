#include "BezierPatchC2.h"

#include <Visitors/AbstractVisitor.h>

BezierPatchC2::BezierPatchC2(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: BaseBezierPatch(scene, controlPoints)
{
	m_Tag = std::format("Bezier C2 patch {}", Id());

	OnPointModify();

	auto patchMesh = LoadPatchPrimitiveMesh();

	auto pipeline = BuD::ShaderPipeline{};
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_vs.hlsl", { sizeof(dxm::Matrix) });
	pipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_hs.hlsl");
	pipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_ds.hlsl", { sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_ps.hlsl");

	auto instancing = BuD::InstanceComponent{};
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData instanceData = {};

		instanceData.m_Data = (uint8_t*)&m_InstanceData;
		instanceData.m_InstancesCount = 1;
		instanceData.m_Size = sizeof(BezierPatchSegmentInstanceData);

		return instanceData;
	};

	auto renderingPass = BuD::RenderingPass{};
	renderingPass.m_Mesh = patchMesh;
	renderingPass.m_Pipeline = pipeline;
	renderingPass.m_Instancing = instancing;
	renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::WIREFRAME;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		auto camera = scene.ActiveCamera();

		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		ds->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses = { renderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void BezierPatchC2::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}
