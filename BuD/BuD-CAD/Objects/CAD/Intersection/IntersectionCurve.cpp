#include "IntersectionCurve.h"

#include <Visitors/Intersection/CalculatorPointOnSurface.h>

IntersectionCurve::IntersectionCurve(BuD::Scene& scene, std::weak_ptr<SceneObjectCAD> surface, std::vector<IntersectionPoint> intersectionPoints)
	: SceneObjectCAD(scene), m_Surface(surface), m_IntersectionPoints(intersectionPoints)
{
	m_Tag = std::format("Intersection curve {}", Id());
	m_Color = IntersectionCurve::UNSELECTED_COLOR;

	UpdateInstanceData();

	auto meshLoader = BuD::MeshLoader();

	auto mesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT_TESSELLATION,
		{
			{ "INS_POINT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 1, DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_POINT", 2, DXGI_FORMAT_R32G32B32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_POINT", 3, DXGI_FORMAT_R32G32B32_FLOAT, 1, 9 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	BuD::ShaderPipeline shaderPipeline = {};
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/IntersectionCurve/intersection_curve_vs.hlsl", { sizeof(dxm::Matrix) });
	shaderPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("./Resources/Shaders/IntersectionCurve/intersection_curve_hs.hlsl");
	shaderPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("./Resources/Shaders/IntersectionCurve/intersection_curve_ds.hlsl", { sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/IntersectionCurve/intersection_curve_ps.hlsl");

	BuD::InstanceComponent instancing = {};
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData data = {};
		data.m_InstancesCount = m_InstanceData.m_Segments.size();
		data.m_Data = (uint8_t*)m_InstanceData.m_Segments.data();
		data.m_Size = m_InstanceData.m_Segments.size() * sizeof(IntersectionCurveSingleSegment);

		return data;
	};

	BuD::RenderingPass renderingPass = {};
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = shaderPipeline;
	renderingPass.m_Instancing = instancing;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto viewMtx = scene.ActiveCamera()->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		vs->UpdateConstantBuffer(0, &viewMtx, sizeof(dxm::Matrix));
		ds->UpdateConstantBuffer(0, &projMtx, sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses = { renderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void IntersectionCurve::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void IntersectionCurve::UpdateInstanceData()
{
	std::vector<dxm::Vector3> controlPoints(m_IntersectionPoints.size());

	std::unique_ptr<CalculatorParameterized> parameterizedCalculator = std::make_unique<CalculatorPointOnSurface>();

	std::transform(m_IntersectionPoints.begin(), m_IntersectionPoints.end(), controlPoints.begin(),
		[this, &parameterizedCalculator](const IntersectionPoint& point)
		{
			parameterizedCalculator->SetParameter(point.m_Parameter);
			parameterizedCalculator->Visit(m_Surface);

			return parameterizedCalculator->Result();
		}
	);

	auto controlPointsCount = controlPoints.size();
	auto segmentsCount = controlPointsCount - 1;

	if (controlPointsCount == 0 || segmentsCount == 0)
	{
		m_InstanceData.m_Segments.clear();
		return;
	}

	auto loop = false;

	auto& front = controlPoints.front();
	auto& back = controlPoints.back();

	if (front == back)
	{
		loop = true;
		segmentsCount = controlPointsCount;
		controlPointsCount = controlPoints.size() - 1;
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

			auto& controlPoint = controlPoints[index];

			segment.m_ControlPoints[controlPointIndex] = controlPoint;
		}
	}
}
