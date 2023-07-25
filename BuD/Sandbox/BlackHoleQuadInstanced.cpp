#include "BlackHoleQuadInstanced.h"

#include <Objects/MeshLoader/MeshLoader.h>
#include <Shaders/Loader/ShaderLoader.h>

struct CBufferInstancedVS
{
	dxm::Vector4 CameraPos;
	dxm::Matrix InverseMvp;
};

struct CBufferInstancedPS
{
	dxm::Vector3 BlackHolePosition;
	float BlackHoleMass;
};

BlackHoleQuadInstanced::BlackHoleQuadInstanced(BuD::Scene& scene)
	: BlackHoleQuadBase(scene)
{
	constexpr int GRID_SIZE = 30;

	std::vector<BuD::RenderingPass> renderingPasses = {};

	auto meshLoader = BuD::MeshLoader();

	// TODO: add passing instance layout to the mesh loaders
	auto quadMesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::QUAD,
		{
			{ "ROW_X", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "ROW_Y", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 4 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "ROW_Z", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 8 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "ROW_W", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 12 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto vertexShader = BuD::ShaderLoader::VSLoad(L"./Resources/shaders/screenspace_quad_instanced_vs.hlsl", { sizeof(CBufferInstancedVS) });
	auto pixelShader = BuD::ShaderLoader::PSLoad(L"./Resources/shaders/blackhole_ps.hlsl", { sizeof(CBufferInstancedPS) });

	BuD::ShaderPipeline pipeline;
	pipeline.m_VertexShader = vertexShader;
	pipeline.m_PixelShader = pixelShader;

	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			int i = GRID_SIZE * y + x;

			BuD::RenderingPass renderingPass = {};

			auto realScale = 1.0f / GRID_SIZE;
			auto scale = 1.0f / (GRID_SIZE + 1);
			auto xPos = -1 + 2 * static_cast<float>(x) / (GRID_SIZE)+realScale;
			auto yPos = -1 + 2 * static_cast<float>(y) / (GRID_SIZE)+realScale;

			auto model = dxm::Matrix::CreateScale(scale) * dxm::Matrix::CreateTranslation({ xPos, yPos, 0.0f });

			m_InstancesData.push_back(model);

			BuD::InstanceComponent instance;
			instance.m_InstanceCallback = [this, i]()
			{
				BuD::InstanceData data = {};
				data.m_Data = (uint8_t*)(&m_InstancesData[i]);
				data.m_Size = sizeof(dxm::Matrix);

				return data;
			};

			renderingPass.m_Mesh = quadMesh;
			renderingPass.m_Pipeline = pipeline;
			renderingPass.m_Instancing = instance;

			renderingPass.m_PreRenderCallback = [this, x, y, GRID_SIZE](const BuD::RenderingPass& renderPass, const BuD::Scene& scene)
			{
				CBufferInstancedVS cbuffer = {};

				auto camera = scene.ActiveCamera();

				cbuffer.CameraPos = dxm::Vector4(camera->EyePosition());

				auto viewMtx = camera->ViewMatrix();
				auto projMtx = BuD::Renderer::ProjectionMatrix();

				auto mvp = viewMtx * projMtx;
				cbuffer.InverseMvp = mvp.Invert();

				auto& vs = renderPass.m_Pipeline.m_VertexShader;
				auto& ps = renderPass.m_Pipeline.m_PixelShader;

				vs->UpdateConstantBuffer(0, &cbuffer, sizeof(CBufferInstancedVS));

				CBufferInstancedPS cbuffer2 = {};

				cbuffer2.BlackHolePosition = m_BlackHolePosition;
				cbuffer2.BlackHoleMass = m_BlackHoleMass;

				ps->UpdateConstantBuffer(0, &cbuffer2, sizeof(CBufferInstancedPS));

				ID3D11ShaderResourceView* SRVs[1] = {};

				switch (m_ActiveCubemap)
				{
				case LINUS:
					SRVs[0] = m_LinusSkybox.SRV();
					break;
				case GALAXY:
				default:
					SRVs[0] = m_GalaxySkybox.SRV();
					break;
				}

				auto graphicsDevice = BuD::Renderer::Device();
				auto& context = graphicsDevice->Context();

				context->PSSetShaderResources(0, 1, SRVs);
			};

			renderingPasses.push_back(renderingPass);
		}
	}

	m_BlackHoleEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}
