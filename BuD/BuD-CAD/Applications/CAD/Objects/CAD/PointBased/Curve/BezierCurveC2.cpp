#include "BezierCurveC2.h"

#include <Applications/CAD/Visitors/AbstractVisitor.h>

BezierCurveC2::BezierCurveC2(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: BaseCurve(scene, controlPoints)
{
	OnPointModify();
	m_Tag = std::format("C2 Bezier curve {}", m_SceneEntity.Id());

	auto meshLoader = BuD::MeshLoader();

	auto curveMesh = LoadCurvePrimitiveMesh();
	auto borderMesh = LoadBorderPrimitiveMesh();

	BuD::ShaderPipeline curveShaderPipeline = {};
	curveShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_vs.hlsl", { sizeof(dxm::Matrix) });
	curveShaderPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_hs.hlsl");
	curveShaderPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_ds.hlsl", { sizeof(dxm::Matrix) });
	curveShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_ps.hlsl");

	BuD::ShaderPipeline bsplineBorderShaderPipeline = {};
	bsplineBorderShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_vs.hlsl", { sizeof(dxm::Matrix) });
	bsplineBorderShaderPipeline.m_GeometryShader = BuD::ShaderLoader::GSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_border_bspline_gs.hlsl", { sizeof(dxm::Matrix) });
	bsplineBorderShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_border_bspline_ps.hlsl");

	BuD::ShaderPipeline bernsteinShaderPipeline = {};
	bernsteinShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_vs.hlsl", { sizeof(dxm::Matrix) });
	bernsteinShaderPipeline.m_GeometryShader = BuD::ShaderLoader::GSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_border_bernstein_gs.hlsl", { sizeof(dxm::Matrix) });
	bernsteinShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/BezierCurveC2/bezier_curve_c2_border_bernstein_ps.hlsl");

	BuD::InstanceComponent instancing = CreateCurveInstancingComponent();

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

	BuD::RenderingPass bsplineBorderRenderingPass = {};
	bsplineBorderRenderingPass.m_Mesh = borderMesh;
	bsplineBorderRenderingPass.m_Pipeline = bsplineBorderShaderPipeline;
	bsplineBorderRenderingPass.m_Instancing = instancing;
	bsplineBorderRenderingPass.m_ShouldSkip = !m_ControlPointBorder;
	bsplineBorderRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto viewMtx = scene.ActiveCamera()->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& gs = renderingPass.m_Pipeline.m_GeometryShader;

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		gs->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	BuD::RenderingPass bernsteinBorderRenderingPass = {};
	bernsteinBorderRenderingPass.m_Mesh = borderMesh;
	bernsteinBorderRenderingPass.m_Pipeline = bernsteinShaderPipeline;
	bernsteinBorderRenderingPass.m_Instancing = instancing;
	bernsteinBorderRenderingPass.m_ShouldSkip = !m_ControlPointBorder;
	bernsteinBorderRenderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto viewMtx = scene.ActiveCamera()->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& gs = renderingPass.m_Pipeline.m_GeometryShader;

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		gs->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses = { curveRenderingPass, bsplineBorderRenderingPass, bernsteinBorderRenderingPass };
	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void BezierCurveC2::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void BezierCurveC2::OnPointModify()
{
	auto controlPointCount = m_ControlPoints.size();
	auto segmentsCount = max(controlPointCount - 3, 0);

	m_InstanceData.m_Segments.resize(segmentsCount);

	for (auto segmentIndex = 0; segmentIndex < segmentsCount; segmentIndex++)
	{
		auto& segment = m_InstanceData.m_Segments[segmentIndex];
		segment.m_Color = m_Color;

		for (int controlPointIndex = 0; controlPointIndex < 4; controlPointIndex++)
		{
			auto index = segmentIndex + controlPointIndex;

			auto& controlPoint = m_ControlPoints[index];
			auto controlPointShared = controlPoint.lock();

			segment.m_SegmentControlPoints[controlPointIndex] = controlPointShared->m_Transform.m_Position;
		}
	}
}

void BezierCurveC2::TogglePolygon(bool borderOn)
{
	if (borderOn == m_ControlPointBorder)
	{
		return;
	}

	m_ControlPointBorder = borderOn;

	auto& renderable = m_SceneEntity.GetComponent<BuD::IRenderable>();

	renderable.RenderingPasses[1].m_ShouldSkip = !borderOn;
	renderable.RenderingPasses[2].m_ShouldSkip = !borderOn;
}

dxm::Vector3 BezierCurveC2::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 BezierCurveC2::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
