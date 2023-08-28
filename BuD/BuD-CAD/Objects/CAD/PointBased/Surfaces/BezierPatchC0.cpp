#include "BezierPatchC0.h"

#include <Visitors/AbstractVisitor.h>

BezierPatchC0::BezierPatchC0(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: BaseBezierPatch(scene, controlPoints)
{
	m_Tag = std::format("Bezier C0 patch {}", Id());

	OnPointModify();

	CreateInstanceRenderingPass();
	CreateTrimmedRenderingPass();

	auto polygonMesh = LoadPolygonPrimitiveMesh();

	auto polygonPipeline = BuD::ShaderPipeline {};
	polygonPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierPatchC0/bezier_patch_c0_vs.hlsl", { sizeof(dxm::Matrix) });
	polygonPipeline.m_GeometryShader = BuD::ShaderLoader::GSLoad("./Resources/Shaders/PointBased/BezierPatchC0/bezier_patch_c0_polygon_gs.hlsl", { sizeof(dxm::Matrix) });
	polygonPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierPatchC0/bezier_patch_c0_polygon_ps.hlsl");

	auto instancing = BuD::InstanceComponent{};
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData instanceData = {};
		
		instanceData.m_Data = (uint8_t*)&m_InstanceData;
		instanceData.m_InstancesCount = 1;
		instanceData.m_Size = sizeof(BezierPatchSegmentInstanceData);

		return instanceData;
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

	std::vector<BuD::RenderingPass> renderingPasses = { m_InstanceRenderingPass, polygonRenderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void BezierPatchC0::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void BezierPatchC0::TogglePolygon(bool polygonOn)
{
	if (m_DisplayBezierPolygon == polygonOn)
	{
		return;
	}

	m_DisplayBezierPolygon = polygonOn;

	auto& renderingComponent = m_SceneEntity.GetComponent<BuD::IRenderable>();
	renderingComponent.RenderingPasses[1].m_ShouldSkip = !m_DisplayBezierPolygon;
}

void BezierPatchC0::CreateInstanceRenderingPass()
{
	auto patchMesh = LoadPatchPrimitiveMesh();

	auto patchPipeline = BuD::ShaderPipeline{};
	patchPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierPatchC0/bezier_patch_c0_vs.hlsl", { sizeof(dxm::Matrix) });
	patchPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/PointBased/BezierPatchC0/bezier_patch_c0_hs.hlsl");
	patchPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/PointBased/BezierPatchC0/bezier_patch_c0_ds.hlsl", { sizeof(dxm::Matrix) });
	patchPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierPatchC0/bezier_patch_c0_ps.hlsl");

	auto instancing = BuD::InstanceComponent{};
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData instanceData = {};

		instanceData.m_Data = (uint8_t*)&m_InstanceData;
		instanceData.m_InstancesCount = 1;
		instanceData.m_Size = sizeof(BezierPatchSegmentInstanceData);

		return instanceData;
	};

	m_InstanceRenderingPass = {};
	m_InstanceRenderingPass.m_Mesh = patchMesh;
	m_InstanceRenderingPass.m_Pipeline = patchPipeline;
	m_InstanceRenderingPass.m_Instancing = instancing;
	m_InstanceRenderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	m_InstanceRenderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::WIREFRAME;
	m_InstanceRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		auto camera = scene.ActiveCamera();

		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		ds->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};
}

void BezierPatchC0::CreateTrimmedRenderingPass()
{
	auto meshLoader = BuD::MeshLoader();
	auto patchMesh = meshLoader.LoadPrimitiveMesh(BuD::MeshPrimitiveType::POINT_TESSELLATION);

	auto patchPipeline = BuD::ShaderPipeline{};
	patchPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierPatchC0/Trimmed/bezier_patch_c0_vs.hlsl", { sizeof(dxm::Matrix), sizeof(BezierPatchSegmentInstanceData) });
	patchPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/PointBased/BezierPatchC0/Trimmed/bezier_patch_c0_hs.hlsl");
	patchPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/PointBased/BezierPatchC0/Trimmed/bezier_patch_c0_ds.hlsl", { sizeof(dxm::Matrix) });
	patchPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierPatchC0/Trimmed/bezier_patch_c0_ps.hlsl");

	m_TrimmedRenderingPass = {};
	m_TrimmedRenderingPass.m_Mesh = patchMesh;
	m_TrimmedRenderingPass.m_Pipeline = patchPipeline;
	m_TrimmedRenderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	m_TrimmedRenderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::WIREFRAME;
	m_TrimmedRenderingPass.m_PreRenderCallback = [this](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto device = BuD::Renderer::Device();
		auto& context = device->Context();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		auto camera = scene.ActiveCamera();

		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		vs->UpdateConstantBuffer(1, &m_InstanceData, sizeof(BezierPatchSegmentInstanceData));

		ds->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));

		ID3D11ShaderResourceView* SRVs[1] = { /*m_ParameterSpace.has_value() ? m_ParameterSpace->SRV() :*/ nullptr};

		context->PSSetShaderResources(0, 1, SRVs);
	};
}
