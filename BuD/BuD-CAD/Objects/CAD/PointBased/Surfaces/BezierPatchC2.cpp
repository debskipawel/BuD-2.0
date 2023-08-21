#include "BezierPatchC2.h"

#include <Visitors/AbstractVisitor.h>

BezierPatchC2::BezierPatchC2(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: BaseBezierPatch(scene, controlPoints)
{
	m_Tag = std::format("Bezier C2 patch {}", Id());

	OnPointModify();

	auto patchMesh = LoadPatchPrimitiveMesh();
	auto polygonMesh = LoadPolygonPrimitiveMesh();

	auto patchPipeline = BuD::ShaderPipeline{};
	patchPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_vs.hlsl", { sizeof(dxm::Matrix) });
	patchPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_hs.hlsl");
	patchPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_ds.hlsl", { sizeof(dxm::Matrix) });
	patchPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_ps.hlsl");

	auto polygonPipeline = BuD::ShaderPipeline{};
	polygonPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_vs.hlsl", { sizeof(dxm::Matrix) });
	polygonPipeline.m_GeometryShader = BuD::ShaderLoader::GSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_polygon_gs.hlsl", { sizeof(dxm::Matrix) });
	polygonPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierPatchC2/bezier_patch_c2_polygon_ps.hlsl");

	auto instancing = BuD::InstanceComponent{};
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData instanceData = {};

		instanceData.m_Data = (uint8_t*)&m_InstanceData;
		instanceData.m_InstancesCount = 1;
		instanceData.m_Size = sizeof(BezierPatchSegmentInstanceData);

		return instanceData;
	};

	auto patchRenderingPass = BuD::RenderingPass{};
	patchRenderingPass.m_Mesh = patchMesh;
	patchRenderingPass.m_Pipeline = patchPipeline;
	patchRenderingPass.m_Instancing = instancing;
	patchRenderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	patchRenderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::WIREFRAME;
	patchRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		auto camera = scene.ActiveCamera();

		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		ds->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	auto polygonRenderingPass = BuD::RenderingPass{};
	polygonRenderingPass.m_ShouldSkip = !m_DisplayBezierPolygon;
	polygonRenderingPass.m_Mesh = polygonMesh;
	polygonRenderingPass.m_Pipeline = polygonPipeline;
	polygonRenderingPass.m_Instancing = instancing;
	polygonRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& gs = renderingPass.m_Pipeline.m_GeometryShader;

		auto camera = scene.ActiveCamera();

		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		gs->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses = { patchRenderingPass, polygonRenderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void BezierPatchC2::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void BezierPatchC2::TogglePolygon(bool polygonOn)
{
	if (m_DisplayBezierPolygon == polygonOn)
	{
		return;
	}

	m_DisplayBezierPolygon = polygonOn;

	auto& renderingComponent = m_SceneEntity.GetComponent<BuD::IRenderable>();
	renderingComponent.RenderingPasses[1].m_ShouldSkip = !m_DisplayBezierPolygon;
}
