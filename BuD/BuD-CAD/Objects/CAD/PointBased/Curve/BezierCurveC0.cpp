#include "BezierCurveC0.h"

#include <numeric>

#include <BuD.h>

#include <Visitors/AbstractVisitor.h>

BezierCurveC0::BezierCurveC0(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: PointBasedObjectCAD(scene, controlPoints), m_Border(false)
{
	OnPointModify();
	m_Tag = std::format("C0 Bezier curve {}", m_SceneEntity.Id());

	auto meshLoader = BuD::MeshLoader();

	auto curveMesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_TESSELLATION,
		{
			{ "INS_POINT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_POINT", 2, DXGI_FORMAT_R32G32B32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 3, DXGI_FORMAT_R32G32B32_FLOAT, 1, 9 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);
	
	auto borderMesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_STANDARD,
		{
			{ "INS_POINT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_POINT", 2, DXGI_FORMAT_R32G32B32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 3, DXGI_FORMAT_R32G32B32_FLOAT, 1, 9 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	BuD::ShaderPipeline curveShaderPipeline = {};
	curveShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierCurveC0/bezier_curve_c0_vs.hlsl", { sizeof(dxm::Matrix) });
	curveShaderPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/PointBased/BezierCurveC0/bezier_curve_c0_hs.hlsl");
	curveShaderPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/PointBased/BezierCurveC0/bezier_curve_c0_ds.hlsl", { sizeof(dxm::Matrix) });
	curveShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierCurveC0/bezier_curve_c0_ps.hlsl");

	BuD::ShaderPipeline borderShaderPipeline = {};
	borderShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierCurveC0/bezier_curve_c0_vs.hlsl", { sizeof(dxm::Matrix) });
	borderShaderPipeline.m_GeometryShader = BuD::ShaderLoader::GSLoad("./Resources/Shaders/PointBased/BezierCurveC0/bezier_curve_c0_border_gs.hlsl", { sizeof(dxm::Matrix) });
	borderShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierCurveC0/bezier_curve_c0_border_ps.hlsl");

	BuD::InstanceComponent instancing;
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData instanceData;
		instanceData.m_Data = (uint8_t*)m_InstanceData.m_Segments.data();
		instanceData.m_InstancesCount = m_InstanceData.m_Segments.size();
		instanceData.m_Size = m_InstanceData.m_Segments.size() * sizeof(BezierCurveSegmentC0);

		return instanceData;
	};

	BuD::RenderingPass curveRenderingPass = {};
	curveRenderingPass.m_Mesh = curveMesh;
	curveRenderingPass.m_Pipeline = curveShaderPipeline;
	curveRenderingPass.m_Instancing = instancing;
	curveRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto viewMtx = scene.ActiveCamera()->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		ds->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	BuD::RenderingPass borderRenderingPass = {};
	borderRenderingPass.m_Mesh = borderMesh;
	borderRenderingPass.m_Pipeline = borderShaderPipeline;
	borderRenderingPass.m_Instancing = instancing;
	borderRenderingPass.m_ShouldSkip = !m_Border;
	borderRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto viewMtx = scene.ActiveCamera()->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& gs = renderingPass.m_Pipeline.m_GeometryShader;

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		gs->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses;
	renderingPasses.push_back(curveRenderingPass);
	renderingPasses.push_back(borderRenderingPass);

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void BezierCurveC0::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void BezierCurveC0::OnPointModify()
{
	auto controlPointCount = m_ControlPoints.size();
	auto segmentsCount = ceil(static_cast<float>(controlPointCount - 1) / 3);

	m_InstanceData.m_Segments.resize(segmentsCount);

	for (auto segmentIndex = 0; segmentIndex < segmentsCount; segmentIndex++)
	{
		auto& segment = m_InstanceData.m_Segments[segmentIndex];
		segment.m_Color = m_Color;

		for (int controlPointIndex = 0; controlPointIndex < 4; controlPointIndex++)
		{
			auto index = min(segmentIndex * 3 + controlPointIndex, controlPointCount - 1);
			
			auto& controlPoint = m_ControlPoints[index];
			auto controlPointShared = controlPoint.lock();

			segment.m_SegmentControlPoints[controlPointIndex] = controlPointShared->m_Transform.m_Position;
		}
	}
}

void BezierCurveC0::RenderBorder(bool borderOn)
{
	if (borderOn == m_Border)
	{
		return;
	}

	auto& rendering = m_SceneEntity.GetComponent<BuD::IRenderable>();

	rendering.RenderingPasses[1].m_ShouldSkip = !borderOn;

	m_Border = borderOn;
}

bool BezierCurveC0::RenderBorder() const
{
	return m_Border;
}

dxm::Vector3 BezierCurveC0::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 BezierCurveC0::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
