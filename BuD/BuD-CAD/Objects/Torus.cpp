#include "Torus.h"

#include <Visitors/AbstractVisitor.h>

Torus::Torus(BuD::Scene& scene)
	: Torus(scene, dxm::Vector3::Zero, dxm::Vector3::UnitZ)
{
}

Torus::Torus(BuD::Scene& scene, dxm::Vector3 position, dxm::Vector3 orientation)
	: Torus(scene, position, orientation, 3.0f, 1.0f)
{
}

Torus::Torus(BuD::Scene& scene, dxm::Vector3 position, dxm::Vector3 orientation, float outerRadius, float innerRadius)
	: Torus(scene, position, orientation, outerRadius, innerRadius, 30, 15)
{
}

Torus::Torus(BuD::Scene& scene, dxm::Vector3 position, dxm::Vector3 orientation, float outerRadius, float innerRadius, unsigned int segmentsU, unsigned int segmentsV)
	: SceneObjectCAD(scene)
{
	m_Tag = "Torus";

	m_InstanceData.m_Position = position;
	m_InstanceData.m_Orientation = orientation;
	m_InstanceData.m_InnerRadius = innerRadius;
	m_InstanceData.m_OuterRadius = outerRadius;
	m_InstanceData.m_SegmentsU = segmentsU;
	m_InstanceData.m_SegmentsV = segmentsV;

	auto meshLoader = BuD::MeshLoader();

	auto pointMesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::POINT,
		{
			{ "INS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_ORIENTATION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_RADIUS", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_SEGMENTS", 0, DXGI_FORMAT_R32G32_UINT, 1, 8 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	BuD::ShaderPipeline pipeline;
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Torus/torus_instanced_vs.hlsl");
	pipeline.m_HullShader = BuD::ShaderLoader::HSLoad("Resources/Shaders/Torus/torus_instanced_hs.hlsl");
	pipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("Resources/Shaders/Torus/torus_instanced_ds.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Torus/torus_instanced_ps.hlsl");

	BuD::InstanceComponent instancing;
	instancing.m_InstanceCallback = [this]()
	{
		auto instance = BuD::InstanceData();
		instance.m_Data = (uint8_t*)(&m_InstanceData);
		instance.m_Size = sizeof(TorusInstanceData);

		return instance;
	};

	BuD::RenderingPass renderingPass;
	renderingPass.m_Mesh = pointMesh;
	renderingPass.m_Pipeline = pipeline;
	renderingPass.m_Instancing = instancing;

	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();

		dxm::Matrix matrices[] = { camera->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };
		auto& ds = renderingPass.m_Pipeline.m_DomainShader;

		ds->UpdateConstantBuffer(0, matrices, 2 * sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> passes;
	passes.push_back(renderingPass);

	m_SceneEntity.AddComponent<BuD::IRenderable>(passes);
}

void Torus::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}
