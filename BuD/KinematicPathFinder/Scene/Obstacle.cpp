#include "Obstacle.h"

Obstacle::Obstacle(BuD::Scene& scene, const dxm::Vector2& v1, const dxm::Vector2& v2, const dxm::Vector3& color)
	: m_RectEntity(scene), m_InstanceData(), m_Vertex1(v1), m_Vertex2(v2)
{
	m_InstanceData.m_Color = color;

	UpdateInstanceData();

	auto meshLoader = BuD::MeshLoader();
	auto quadMesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::QUAD,
		{
			{ "INS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, sizeof(m_InstanceData.m_Position), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, sizeof(m_InstanceData.m_Position) + sizeof(m_InstanceData.m_Size), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Obstacle/obstacle_vs.hlsl", { sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Obstacle/obstacle_ps.hlsl");

	auto instancingComponent = BuD::InstanceComponent();
	instancingComponent.m_InstanceCallback = [this]()
	{
		BuD::InstanceData data;

		data.m_Data = reinterpret_cast<uint8_t*>(&m_InstanceData);
		data.m_InstancesCount = 1;
		data.m_Size = sizeof(ObstacleInstance);

		return data;
	};

	auto renderingPass = BuD::RenderingPass();
	renderingPass.m_Mesh = quadMesh;
	renderingPass.m_Pipeline = shaderPipeline;
	renderingPass.m_Instancing = instancingComponent;

	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto aspectRatio = BuD::Renderer::AspectRatio();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		vs->UpdateConstantBuffer(0, &aspectRatio, sizeof(float));
	};

	auto renderingPasses = std::vector<BuD::RenderingPass>{ renderingPass };

	m_RectEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

Obstacle::~Obstacle()
{
	m_RectEntity->DestroyEntity(m_RectEntity);
}

auto Obstacle::UpdateFirstVertex(const dxm::Vector2& v) -> void
{
	m_Vertex1 = v;

	UpdateInstanceData();
}

auto Obstacle::UpdateSecondVertex(const dxm::Vector2& v) -> void
{
	m_Vertex2 = v;

	UpdateInstanceData();;
}

auto Obstacle::Position() const -> dxm::Vector2
{
	return { m_InstanceData.m_Position.x, m_InstanceData.m_Position.y };
}

auto Obstacle::Size() const -> dxm::Vector2
{
	return m_InstanceData.m_Size;
}

auto Obstacle::Color() -> dxm::Vector3&
{
	return m_InstanceData.m_Color;
}

auto Obstacle::UpdateInstanceData() -> void
{
	m_InstanceData.m_Position = { 0.5f * (m_Vertex1.x + m_Vertex2.x), 0.5f * (m_Vertex1.y + m_Vertex2.y), -0.5f };
	m_InstanceData.m_Size = { fabsf(m_Vertex1.x - m_Vertex2.x), fabsf(m_Vertex1.y - m_Vertex2.y) };
}
