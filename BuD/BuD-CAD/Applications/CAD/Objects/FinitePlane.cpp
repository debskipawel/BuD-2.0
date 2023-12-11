#include "FinitePlane.h"

#include <Applications/CAD/Visitors/AbstractVisitor.h>

FinitePlane::FinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal, float widthU, float widthV)
	: Plane(scene, point, normal), m_WidthU(widthU), m_WidthV(widthV)
{
	m_Tag = "Finite plane";

	InitializeRenderComponent();
}

FinitePlane::FinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& dU, const dxm::Vector3& dV, float widthU, float widthV)
	: Plane(scene, point, dU, dV), m_WidthU(widthU), m_WidthV(widthV)
{
	m_Tag = "Finite plane";

	InitializeRenderComponent();
}

auto FinitePlane::Accept(AbstractVisitor& visitor) -> void
{
	visitor.Visit(*this);
}

auto FinitePlane::GetPoint(float u, float v) const -> dxm::Vector3
{
	if (fabsf(u) > 0.5f * m_WidthU || fabsf(v) > 0.5f * m_WidthV)
	{
		return dxm::Vector3(NAN);
	}

	return m_Transform.m_Position + u * m_VectorU + v * m_VectorV;
}

auto FinitePlane::InitializeRenderComponent() -> void
{
	UpdateInstanceData();

	auto meshLoader = BuD::MeshLoader();
	auto quadMesh = meshLoader.LoadPrimitiveMesh(BuD::MeshPrimitiveType::QUAD,
		{
			{ "INS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_DERIVATIVE_U", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, sizeof(dxm::Vector3), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_DERIVATIVE_V", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 2 * sizeof(dxm::Vector3), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 3 * sizeof(dxm::Vector3), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/FinitePlane/finite_plane_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/FinitePlane/finite_plane_ps.hlsl");

	auto instanceComponent = BuD::InstanceComponent();
	instanceComponent.m_InstanceCallback = [this]()
	{
		auto data = BuD::InstanceData();

		data.m_Data = reinterpret_cast<uint8_t*>(&m_InstanceData);
		data.m_InstancesCount = 1;
		data.m_Size = sizeof(m_InstanceData);

		return data;
	};

	auto renderingPass = BuD::RenderingPass();
	renderingPass.m_Mesh = quadMesh;
	renderingPass.m_Pipeline = shaderPipeline;
	renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::SOLID;
	renderingPass.m_Instancing = instanceComponent;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();

		auto vsCB = std::array<dxm::Matrix, 2>{ camera->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		vs->UpdateConstantBuffer(0, vsCB.data(), vsCB.size() * sizeof(dxm::Matrix));
	};

	auto renderingPasses = std::vector<BuD::RenderingPass>{ renderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

auto FinitePlane::UpdateInstanceData() -> void
{
	m_InstanceData.m_Position = m_Transform.m_Position;
	
	m_InstanceData.m_VectorU = m_VectorU;
	m_InstanceData.m_VectorV = m_VectorV;

	m_InstanceData.m_Size = dxm::Vector2(m_WidthU, m_WidthV);
}
