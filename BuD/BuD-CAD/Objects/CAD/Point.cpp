#include "Point.h"

#include <Objects/CAD/PointBased/PointBasedObjectCAD.h>
#include <Visitors/AbstractVisitor.h>

Point::Point(BuD::Scene& scene)
	: Point(scene, dxm::Vector3::Zero)
{
}

Point::Point(BuD::Scene& scene, dxm::Vector3 position)
	: SceneObjectCAD(scene)
{
	m_Tag = std::format("Point {}", m_SceneEntity.Id());

	m_Transform.m_Position = position;
	m_InstanceData.m_Position = position;
	m_InstanceData.m_Color = Point::UNSELECTED_COLOR;

	BuD::MeshLoader meshLoader;
	
	auto mesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::QUAD,
		{ 
			{ "INS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
		}
	);
	mesh.m_BoundingBoxCallback = [this]()
	{
		BuD::AABB aabb = {};
		aabb.m_Min = aabb.m_Max = m_InstanceData.m_Position;

		return aabb;
	};

	BuD::ShaderPipeline pipeline;
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Point/point_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Point/point_ps.hlsl");

	BuD::InstanceComponent instancing;
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData data = {};
		data.m_Data = (uint8_t*)&m_InstanceData;
		data.m_Size = sizeof(PointInstanceData);

		return data;
	};

	BuD::RenderingPass renderingPass = {};
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = pipeline;
	renderingPass.m_Instancing = instancing;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto viewMtx = scene.ActiveCamera()->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		dxm::Matrix vsCB[] = { viewMtx, projMtx };

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses = {};
	renderingPasses.push_back(renderingPass);

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void Point::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

dxm::Vector3 Point::SELECTED_COLOR = { 0.8f, 0.6f, 0.0f };
dxm::Vector3 Point::UNSELECTED_COLOR = { 1.0f, 1.0f, 1.0f };
