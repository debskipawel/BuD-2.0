#include "FrameMesh.h"

FrameMesh::FrameMesh(BuD::Scene& scene, const Frame& frame)
	: m_FrameEntity(scene)
{
	UpdateMesh(frame);

	auto meshLoader = BuD::MeshLoader();

	auto arrowMeshElements = meshLoader.LoadMeshFromFile("Resources/Meshes/unit_arrow.obj",
		{
			{ "INS_MODEL_X", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_Y", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_Z", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 2 * sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_MODEL_W", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 3 * sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "INS_COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 4 * sizeof(dxm::Vector4), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto sphereMeshElements = meshLoader.LoadMeshFromFile("Resources/Meshes/unit_sphere.obj",
		{ { "INS_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 }}
	);

	auto arrowShaderPipeline = BuD::ShaderPipeline();
	arrowShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Arrow/arrow_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	arrowShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Arrow/arrow_ps.hlsl");

	auto sphereShaderPipeline = BuD::ShaderPipeline();
	sphereShaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/Sphere/sphere_vs.hlsl", { 2 * sizeof(dxm::Matrix) });
	sphereShaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/Sphere/sphere_ps.hlsl");

	auto renderingPasses = std::vector<BuD::RenderingPass>();

	for (const auto& arrowMeshElement : arrowMeshElements)
	{
		auto renderingPass = BuD::RenderingPass();

		renderingPass.m_Mesh = arrowMeshElement.m_Details;
		renderingPass.m_Pipeline = arrowShaderPipeline;
		renderingPass.m_Instancing = { 
			[this]()
			{
				auto data = BuD::InstanceData();

				data.m_InstancesCount = m_InstanceData.size();
				data.m_Data = reinterpret_cast<uint8_t*>(m_InstanceData.data());
				data.m_Size = m_InstanceData.size() * sizeof(FrameArrowInstanceData);

				return data;
			}
		};
		renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
		{
			auto& vs = renderingPass.m_Pipeline.m_VertexShader;

			dxm::Matrix vsCB[] = { scene.ActiveCamera()->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };

			vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));
		};
		renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::FRONT;
		renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::SOLID;

		renderingPasses.push_back(renderingPass);
	}

	for (const auto& sphereMeshElement : sphereMeshElements)
	{
		auto renderingPass = BuD::RenderingPass();

		renderingPass.m_Mesh = sphereMeshElement.m_Details;
		renderingPass.m_Pipeline = sphereShaderPipeline;
		renderingPass.m_Instancing = {
			[this]()
			{
				auto data = BuD::InstanceData();

				data.m_InstancesCount = 1;
				data.m_Data = reinterpret_cast<uint8_t*>(&m_FramePosition);
				data.m_Size = sizeof(m_FramePosition);

				return data;
			}
		};
		renderingPass.m_PreRenderCallback = [](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
		{
			auto& vs = renderingPass.m_Pipeline.m_VertexShader;

			dxm::Matrix vsCB[] = { scene.ActiveCamera()->ViewMatrix(), BuD::Renderer::ProjectionMatrix() };

			vs->UpdateConstantBuffer(0, vsCB, 2 * sizeof(dxm::Matrix));
		};
		renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::FRONT;
		renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::SOLID;

		renderingPasses.push_back(renderingPass);
	}

	m_FrameEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

auto FrameMesh::UpdateMesh(const Frame& frame) -> void
{
	m_FramePosition = frame.Position();

	auto right = frame.Right();
	auto up = frame.Up();
	auto front = frame.Front();

	auto rightModelMtx = dxm::Matrix::CreateScale(1.0f, right.Length(), 1.0f) 
		* dxm::Matrix::CreateFromQuaternion(dxm::Quaternion::FromToRotation(dxm::Vector3::UnitY, right)) 
		* dxm::Matrix::CreateTranslation(frame.Position());
	
	auto upModelMtx = dxm::Matrix::CreateScale(1.0f, up.Length(), 1.0f)
		* dxm::Matrix::CreateFromQuaternion(dxm::Quaternion::FromToRotation(dxm::Vector3::UnitY, up))
		* dxm::Matrix::CreateTranslation(frame.Position());

	auto frontModelMtx = dxm::Matrix::CreateScale(1.0f, front.Length(), 1.0f)
		* dxm::Matrix::CreateFromQuaternion(dxm::Quaternion::FromToRotation(dxm::Vector3::UnitY, front))
		* dxm::Matrix::CreateTranslation(frame.Position());

	m_InstanceData[0] = { rightModelMtx, dxm::Vector4(1.0f, 0.0f, 0.0f, 1.0f) };
	m_InstanceData[1] = { upModelMtx, dxm::Vector4(0.0f, 1.0f, 0.0f, 1.0f) };
	m_InstanceData[2] = { frontModelMtx, dxm::Vector4(0.0f, 0.0f, 1.0f, 1.0f) };
}
