#include "YukselInterpolatingCurveC2.h"

#include <Visitors/AbstractVisitor.h>

YukselInterpolatingCurveC2::YukselInterpolatingCurveC2(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: BaseCurve(scene, controlPoints)
{
	OnPointModify();
	m_Tag = std::format("C2 Yuksel interpolation curve {}", m_SceneEntity.Id());

	auto meshLoader = BuD::MeshLoader();

	auto curveMesh = LoadCurvePrimitiveMesh();

	BuD::ShaderPipeline curveShaderPipeline = {};
	curveShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/PointBased/YukselCurveC2/yuksel_curve_c2_vs.hlsl", { sizeof(dxm::Matrix) });
	curveShaderPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/PointBased/YukselCurveC2/yuksel_curve_c2_hs.hlsl");
	curveShaderPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/PointBased/YukselCurveC2/yuksel_curve_c2_ds.hlsl", { sizeof(dxm::Matrix) });
	curveShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/PointBased/YukselCurveC2/yuksel_curve_c2_ps.hlsl");

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

	std::vector<BuD::RenderingPass> renderingPasses = { curveRenderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void YukselInterpolatingCurveC2::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void YukselInterpolatingCurveC2::OnPointModify()
{
	auto controlPointsCount = m_ControlPoints.size();
	auto segmentsCount = controlPointsCount - 1;

	auto loop = false;

	auto& front = m_ControlPoints.front();
	auto& back = m_ControlPoints.back();

	auto frontShared = front.lock();
	auto backShared = back.lock();

	if (frontShared && frontShared == backShared)
	{
		loop = true;
		segmentsCount = controlPointsCount;
	}

	m_InstanceData.m_Segments.resize(segmentsCount);

	for (int segmentIndex = 0; segmentIndex < segmentsCount; segmentIndex++)
	{
		auto& segment = m_InstanceData.m_Segments[segmentIndex];
		segment.m_Color = m_Color;

		for (int controlPointIndex = 0; controlPointIndex < 4; controlPointIndex++)
		{
			auto index = segmentIndex + controlPointIndex - 1;
			
			if (loop)
			{
				index = (index + controlPointsCount) % controlPointsCount;
			}

			index = max(index, 0);
			index = min(index, controlPointsCount - 1);

			auto& controlPoint = m_ControlPoints[index];
			auto controlPointShared = controlPoint.lock();

			dxm::Vector3 position = controlPointShared->m_Transform.m_Position;

			segment.m_SegmentControlPoints[controlPointIndex] = position;
		}
	}
}

void YukselInterpolatingCurveC2::RenderControlPointBorder(bool borderOn)
{
	return;
}

dxm::Vector3 YukselInterpolatingCurveC2::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 YukselInterpolatingCurveC2::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
