#include "BlackHoleQuadStandard.h"

#include <Shaders/Loader/ShaderLoader.h>

#include <Objects/MeshLoader/MeshLoader.h>

struct CBufferVS
{
	dxm::Vector4 CameraPos;
	dxm::Matrix InverseMvp;
	dxm::Matrix ModelMatrix;
};

struct CBufferPS
{
	dxm::Vector3 BlackHolePosition;
	float BlackHoleMass;
};

BlackHoleQuadStandard::BlackHoleQuadStandard(BuD::Scene& scene)
	: BlackHoleQuadBase(scene)
{
	constexpr int GRID_SIZE = 30;

	std::vector<BuD::RenderingPass> renderingPasses = {};
	
	auto meshLoader = BuD::MeshLoader();
	auto quadMesh = meshLoader.LoadPrimitiveMesh(BuD::MeshPrimitiveType::QUAD);

	auto vertexShader = BuD::ShaderLoader::VSLoad(L"./Resources/shaders/screenspace_quad_vs.hlsl", { sizeof(CBufferVS) });
	auto pixelShader = BuD::ShaderLoader::PSLoad(L"./Resources/shaders/blackhole_ps.hlsl", { sizeof(CBufferPS) });

	for (int y = 0; y < GRID_SIZE; y++)
	{
		for (int x = 0; x < GRID_SIZE; x++)
		{
			BuD::RenderingPass renderPass = {};

			BuD::ShaderPipeline pipeline = {};

			pipeline.m_VertexShader = vertexShader;
			pipeline.m_PixelShader = pixelShader;

			renderPass.m_Mesh = quadMesh;
			renderPass.m_Pipeline = pipeline;

			renderPass.m_PreRenderCallback = [this, x, y, GRID_SIZE](const BuD::RenderingPass& renderPass, const BuD::Scene& scene)
			{
				CBufferVS cbuffer = {};

				auto camera = scene.ActiveCamera();

				cbuffer.CameraPos = dxm::Vector4(camera->EyePosition());

				auto viewMtx = camera->ViewMatrix();
				auto projMtx = BuD::Renderer::ProjectionMatrix();

				auto mvp = viewMtx * projMtx;
				cbuffer.InverseMvp = mvp.Invert();

				auto realScale = 1.0f / GRID_SIZE;
				auto scale = 1.0f / (GRID_SIZE + 1);
				auto xPos = -1 + 2 * static_cast<float>(x) / (GRID_SIZE) + realScale;
				auto yPos = -1 + 2 * static_cast<float>(y) / (GRID_SIZE) + realScale;

				cbuffer.ModelMatrix = dxm::Matrix::CreateScale(scale) * dxm::Matrix::CreateTranslation({ xPos, yPos, 0.0f });

				auto& vs = renderPass.m_Pipeline.m_VertexShader;
				auto& ps = renderPass.m_Pipeline.m_PixelShader;

				vs->UpdateConstantBuffer(0, &cbuffer, sizeof(CBufferVS));

				CBufferPS cbuffer2 = {};

				cbuffer2.BlackHolePosition = m_BlackHolePosition;
				cbuffer2.BlackHoleMass = m_BlackHoleMass;

				ps->UpdateConstantBuffer(0, &cbuffer2, sizeof(CBufferPS));

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

			renderingPasses.push_back(renderPass);
		}
	}

	m_BlackHoleEntity = BuD::SceneEntity(scene);

	m_BlackHoleEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}