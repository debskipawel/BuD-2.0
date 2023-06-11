#include "BlackHoleQuad.h"

#include <Buffers/VertexBuffer.h>
#include <Buffers/IndexBuffer.h>
#include <Shaders/Loader/ShaderLoader.h>

std::vector<D3D11_INPUT_ELEMENT_DESC> quadLayout =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

std::vector<dxm::Vector2> quadVertices =
{
	{ -1.0f, -1.0f },
	{ -1.0f,  1.0f },
	{  1.0f, -1.0f },
	{  1.0f,  1.0f },
};

std::vector<unsigned short> quadIndices =
{
	0, 3, 1,
	0, 2, 3
};

struct CBufferVS
{
	dxm::Vector4 CameraPos;
	dxm::Matrix InverseMvp;
};

struct CBufferPS
{
	dxm::Vector3 BlackHolePosition;
	float BlackHoleMass;
};

BlackHoleQuad::BlackHoleQuad(BuD::Scene& scene)
	: m_BlackHoleMass(5.0f)
{
	std::vector<BuD::RenderingPass> renderingPasses = {};
	BuD::RenderingPass renderPass = {};

	m_GalaxySkybox = BuD::Texture::LoadFromFile("Resources/textures/galaxy_red.dds");
	m_LinusSkybox = BuD::Texture::LoadFromFile("Resources/textures/linus_cubemap.dds");

	renderPass.VertexBuffer = std::make_shared<BuD::VertexBuffer>(quadVertices.size() * sizeof(dxm::Vector2), quadLayout, quadVertices.data());
	renderPass.IndexBuffer = std::make_shared<BuD::IndexBuffer>(DXGI_FORMAT_R16_UINT, quadIndices.size() * sizeof(unsigned short), quadIndices.data());

	renderPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/screenspace_quad_vs.hlsl", quadLayout, { sizeof(CBufferVS) });
	renderPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/blackhole_ps.hlsl", { sizeof(CBufferPS) });

	renderPass.PreRenderCallback = [this](const BuD::RenderingPass& renderPass, BuD::SceneEntity entity)
	{
		CBufferVS cbuffer = {};
		
		auto camera = entity->ActiveCamera();

		cbuffer.CameraPos = dxm::Vector4(camera->EyePosition());

		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		auto mvp = viewMtx * projMtx;
		cbuffer.InverseMvp = mvp.Invert();

		auto& vs = renderPass.VertexShader;
		auto& ps = renderPass.PixelShader;

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

	m_BlackHoleEntity = BuD::SceneEntity(scene);

	m_BlackHoleEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}