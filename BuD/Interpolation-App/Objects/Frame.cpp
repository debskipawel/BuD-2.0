#include "Frame.h"

Frame::Frame(BuD::Scene& scene)
	: m_FrameEntity(scene), m_Model(dxm::Matrix::Identity)
{
	auto meshLoader = BuD::MeshLoader();
	auto mesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::CUBE,
		{
			{ "INS_MODEL_X", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_Y", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 4 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_Z", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 8 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_W", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/frame_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/frame_ps.hlsl", { 2 * sizeof(dxm::Matrix) });

	auto instanceComponent = BuD::InstanceComponent();
	instanceComponent.m_InstanceCallback = [this]()
	{
		auto data = BuD::InstanceData();

		data.m_Data = reinterpret_cast<uint8_t*>(&m_Model);
		data.m_InstancesCount = 1;
		data.m_Size = sizeof(dxm::Matrix);

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
		auto viewMtx = scene.ActiveCamera()->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		dxm::Matrix cb[] = { viewMtx, projMtx };

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		auto& ps = renderingPass.m_Pipeline.m_PixelShader;

		vs->UpdateConstantBuffer(0, cb, 2 * sizeof(dxm::Matrix));
		ps->UpdateConstantBuffer(0, cb, 2 * sizeof(dxm::Matrix));
	};

	auto renderingPasses = std::vector<BuD::RenderingPass>{ renderingPass };

	m_FrameEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void Frame::SetModelMatrix(const dxm::Matrix& matrix)
{
	m_Model = matrix;
}
