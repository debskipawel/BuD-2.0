#include "InfinitePlane.h"

#include <Applications/CAD/Visitors/AbstractVisitor.h>

InfinitePlane::InfinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& normal)
    : Plane(scene, point, normal)
{
    m_Tag = "Infinite plane";

    InitializeRenderComponent();
}

InfinitePlane::InfinitePlane(BuD::Scene& scene, const dxm::Vector3& point, const dxm::Vector3& dU, const dxm::Vector3& dV)
    : Plane(scene, point, dU, dV)
{
    m_Tag = "Infinite plane";

    InitializeRenderComponent();
}

auto InfinitePlane::Accept(AbstractVisitor& visitor) -> void
{
    visitor.Visit(*this);
}

auto InfinitePlane::GetPoint(float u, float v) const -> dxm::Vector3
{
    return m_Transform.m_Position + u * m_VectorU + v * m_VectorV;
}

auto InfinitePlane::UpdateInstanceData() -> void
{
	m_InstanceData.m_Position = m_Transform.m_Position;
	m_InstanceData.m_Normal = GetNormal();
}

auto InfinitePlane::InitializeRenderComponent() -> void
{
	UpdateInstanceData();

	auto meshLoader = BuD::MeshLoader();
	auto mesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::QUAD,
		{
			{ "INS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, sizeof(dxm::Vector3), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("./Resources/Shaders/InfinitePlane/infinite_plane_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("./Resources/Shaders/InfinitePlane/infinite_plane_ps.hlsl", { 2 * sizeof(dxm::Matrix) });

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
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = shaderPipeline;
	renderingPass.m_Instancing = instanceComponent;
	renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::SOLID;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();

		auto viewMtx = camera->ViewMatrix();
		auto invViewMtx = viewMtx.Invert();

		auto projMtx = BuD::Renderer::ProjectionMatrix();
		auto invProjMtx = projMtx.Invert();

		dxm::Matrix vsCB[] = { invViewMtx, invProjMtx };
		dxm::Matrix psCB[] = { viewMtx, projMtx };

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));

		auto& ps = renderingPass.m_Pipeline.m_PixelShader;
		ps->UpdateConstantBuffer(0, psCB, 2 * sizeof(dxm::Matrix));
	};

	auto renderingPasses = std::vector<BuD::RenderingPass>{ renderingPass };

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}
