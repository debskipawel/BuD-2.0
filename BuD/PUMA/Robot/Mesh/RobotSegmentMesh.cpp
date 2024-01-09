#include "RobotSegmentMesh.h"

#include <numbers>

RobotSegmentMesh::RobotSegmentMesh(BuD::Scene& scene, const dxm::Vector3& position, const dxm::Vector3& orientation, float length, const dxm::Vector3& color)
	: m_SegmentEntity(scene), m_SegmentInstanceData()
{
	UpdateInstanceData(position, orientation, length, color);

	auto meshLoader = BuD::MeshLoader();
	auto mesh = meshLoader.LoadMeshFromFile(
		"Resources/Meshes/cylinder.obj",
		{
			{ "INS_MODEL_X", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_Y", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_MODEL_Z", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 2 * sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_W", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 3 * sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 4 * sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1},
		}
	);

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/robot_segment_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/robot_segment_ps.hlsl");

	auto instanceComponent = BuD::InstanceComponent();
	instanceComponent.m_InstanceCallback = [this]()
	{
		auto data = BuD::InstanceData();

		data.m_Data = reinterpret_cast<uint8_t*>(&m_SegmentInstanceData);
		data.m_InstancesCount = 1;
		data.m_Size = sizeof(m_SegmentInstanceData);

		return data;
	};

	auto renderingPasses = std::vector<BuD::RenderingPass>();

	for (const auto& meshSegment : mesh)
	{
		auto renderingPass = BuD::RenderingPass();
		renderingPass.m_Mesh = meshSegment.m_Details;
		renderingPass.m_Pipeline = shaderPipeline;
		renderingPass.m_Instancing = instanceComponent;
		renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::SOLID;
		renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::FRONT;
		renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
		{
			auto camera = scene.ActiveCamera();

			dxm::Matrix vsCB[] = { camera->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };

			auto& vs = renderingPass.m_Pipeline.m_VertexShader;
			vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));
		};

		renderingPasses.push_back(renderingPass);
	}

	m_SegmentEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

auto RobotSegmentMesh::UpdateInstanceData(const dxm::Vector3& position, const dxm::Vector3& orientation, float length, const dxm::Vector3& color) -> void
{
	UpdateInstanceData(position, orientation, length);
	
	m_SegmentInstanceData.m_Color = color;
}

auto RobotSegmentMesh::UpdateInstanceData(const dxm::Vector3& position, const dxm::Vector3& orientation, float length) -> void
{
	auto cylinderAxis = dxm::Vector3::UnitY;

	if (1.0f - cylinderAxis.Dot(orientation) < 1e-5f)
	{
		auto modelMatrix = dxm::Matrix::CreateScale(0.2f, length, 0.2f) * dxm::Matrix::CreateTranslation(position);

		m_SegmentInstanceData.m_ModelMatrix = modelMatrix;
	}
	else if (1.0f + cylinderAxis.Dot(orientation) < 1e-5f)
	{
		auto rotation = dxm::Quaternion::CreateFromAxisAngle(dxm::Vector3::UnitX, std::numbers::pi_v<float>);

		auto modelMatrix = dxm::Matrix::CreateScale(0.2f, length, 0.2f) * dxm::Matrix::CreateFromQuaternion(rotation) * dxm::Matrix::CreateTranslation(position);

		m_SegmentInstanceData.m_ModelMatrix = modelMatrix;
	}
	else
	{
		auto rotation = dxm::Quaternion::FromToRotation(cylinderAxis, orientation);

		auto modelMatrix = dxm::Matrix::CreateScale(0.2f, length, 0.2f) * dxm::Matrix::CreateFromQuaternion(rotation) * dxm::Matrix::CreateTranslation(position);

		m_SegmentInstanceData.m_ModelMatrix = modelMatrix;
	}
}
