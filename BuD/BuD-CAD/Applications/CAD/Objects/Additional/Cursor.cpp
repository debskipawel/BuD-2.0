#include "Cursor.h"

Cursor::Cursor(BuD::Scene& scene)
	: m_SceneEntity(scene)
{
	BuD::MeshLoader meshLoader;
	auto mesh = meshLoader.LoadPrimitiveMesh(BuD::MeshPrimitiveType::CUBE, 
		{
			{ "INS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_APP_STATE", 0, DXGI_FORMAT_R32_SINT, 1, 3 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1},
			{ "INS_AXIS_LOCK", 0, DXGI_FORMAT_R32_SINT, 1, 3 * sizeof(float) + sizeof(int), D3D11_INPUT_PER_INSTANCE_DATA, 1},
		}
	);

	BuD::ShaderPipeline pipeline;
	pipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Cursor/cursor_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	pipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Cursor/cursor_ps.hlsl", { 2 * sizeof(dxm::Matrix) });

	BuD::InstanceComponent instancing;
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData instanceData = {};
		instanceData.m_Data = (uint8_t*)&m_InstanceData;
		instanceData.m_Size = sizeof(CursorInstanceData);

		return instanceData;
	};

	BuD::RenderingPass renderingPass;
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = pipeline;
	renderingPass.m_Instancing = instancing;
	renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();
		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		dxm::Matrix vsCB[] = { viewMtx, projMtx };

		auto& vs = renderingPass.m_Pipeline.m_VertexShader;
		vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));

		auto& ps = renderingPass.m_Pipeline.m_PixelShader;
		ps->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));
	};

	std::vector<BuD::RenderingPass> renderingPasses;
	renderingPasses.push_back(renderingPass);

	m_SceneEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

dxm::Vector3 Cursor::GetPosition() const
{
	return m_InstanceData.m_Position;
}

void Cursor::SetPosition(const dxm::Vector3& position)
{
	m_InstanceData.m_Position = position;
}

void Cursor::SetAppState(AppState state)
{
	m_InstanceData.m_AppState = state;
}

void Cursor::SetAxisLock(AxisLock axisLock)
{
	m_InstanceData.m_AxisLock = axisLock;
}
