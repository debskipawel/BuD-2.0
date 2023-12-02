#include "RobotArm.h"

RobotArm::RobotArm(BuD::Scene& scene, const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2)
	: m_RobotArmEntity(scene)
{
	UpdateRobotPoints(p0, p1, p2);

	auto meshLoader = BuD::MeshLoader();
	auto quadMesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::QUAD,
		{
			{ "INS_POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_ORIENTATION", 0, DXGI_FORMAT_R32G32_FLOAT, 1, sizeof(InstanceData::m_Position), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_LENGTH", 0, DXGI_FORMAT_R32_FLOAT, 1, sizeof(InstanceData::m_Position) + sizeof(InstanceData::m_Orientation), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Robot/robot_arm_vs.hlsl", { sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Robot/robot_arm_ps.hlsl");

	auto instanceComponent = BuD::InstanceComponent();
	instanceComponent.m_InstanceCallback = [this]()
	{
		BuD::InstanceData data;

		data.m_Data = reinterpret_cast<uint8_t*>(m_InstanceData.data());
		data.m_InstancesCount = m_InstanceData.size();
		data.m_Size = m_InstanceData.size() * sizeof(InstanceData);

		return data;
	};

	auto renderingPass = BuD::RenderingPass();
	renderingPass.m_Mesh = quadMesh;
	renderingPass.m_Pipeline = shaderPipeline;
	renderingPass.m_Instancing = instanceComponent;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto aspectRatio = BuD::Renderer::AspectRatio();

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		vs->UpdateConstantBuffer(0, &aspectRatio, sizeof(aspectRatio));
	};

	auto renderingPasses = std::vector<BuD::RenderingPass>{ renderingPass };

	m_RobotArmEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

auto RobotArm::UpdateRobotPoints(const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2) -> void
{
	m_InstanceData[0].m_Position = 0.5f * (p0 + p1);
	m_InstanceData[1].m_Position = 0.5f * (p1 + p2);
	
	auto orientation1 = p1 - p0;
	auto orientation2 = p2 - p1;

	auto squaredL1 = orientation1.LengthSquared();
	auto squaredL2 = orientation2.LengthSquared();

	if (squaredL1 > 1e-8f)
	{
		m_InstanceData[0].m_Length = sqrtf(squaredL1);
		m_InstanceData[0].m_Orientation = orientation1 / m_InstanceData[0].m_Length;
	}

	if (squaredL2 > 1e-8f)
	{
		m_InstanceData[1].m_Length = sqrtf(squaredL2);
		m_InstanceData[1].m_Orientation = orientation2 / m_InstanceData[1].m_Length;
	}
}
