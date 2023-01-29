#include "GaussianBlurFaceMesh.h"

#include <Objects/Interfaces/IEmissive.h>
#include <Resources/Meshes/Quad.h>
#include <Shaders/Loader/ShaderLoader.h>

struct MaterialCb
{
	dxm::Vector4 AmbientColor;
	dxm::Vector4 DiffuseColor;
	dxm::Vector4 SpecularColor;

	float SpecularExponent;
	float OpticalDensity;
	float Dissolve;
	int Illumination;
	dxm::Vector4 exp;
};

dxm::Vector4 toV4(const dxm::Vector3& other)
{
	return { other.x, other.y, other.z, 0.0f };
}

GaussianBlurFaceMesh::GaussianBlurFaceMesh(BuD::Scene& scene)
	: FaceMesh(scene)
{
	auto material = m_MeshSegments[0].Material;
	auto [entity, emissive, state] = *scene.GetAllEntitiesWith<BuD::IEmissive, BuD::IState>().begin();

	BuD::RenderingPass lightPass{};

	lightPass.VertexBuffer = m_MeshSegments[0].VertexBuffer;
	lightPass.IndexBuffer = m_MeshSegments[0].IndexBuffer;

	lightPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/lightMap_vs.hlsl", m_MeshSegments[0].Layout, { 3 * sizeof(Matrix) });
	lightPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/depthMapAbsorption_ps.hlsl");

	lightPass.PreRenderCallback = [this, &state](const BuD::RenderingPass& renderPass, BuD::SceneEntity entity)
	{
		BuD::Renderer::BeginTarget(4000, 4000);
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

		auto& faceState = entity.GetComponent<BuD::IState>();

		auto model = dxm::Matrix::CreateFromQuaternion(faceState.Rotation) * dxm::Matrix::CreateRotationX(DirectX::XM_PIDIV2) * dxm::Matrix::CreateTranslation(faceState.Position);
		auto view = state.Position == faceState.Position
			? dxm::Matrix::Identity
			: dxm::Matrix::CreateLookAt(state.Position, faceState.Position, { 0.0f, 0.0f, 1.0f });
		auto proj = BuD::Renderer::ProjectionMatrix();

		Matrix cb[] =
		{
			proj,
			view,
			model,
		};

		renderPass.VertexShader->UpdateConstantBuffer(0, cb, 3 * sizeof(Matrix));
	};

	m_RenderingPasses.push_back(lightPass);

	BuD::RenderingPass firstPass{};

	firstPass.VertexBuffer = m_MeshSegments[0].VertexBuffer;
	firstPass.IndexBuffer = m_MeshSegments[0].IndexBuffer;

	firstPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/uv_map_vs.hlsl", m_MeshSegments[0].Layout, { 4 * sizeof(Matrix), 2 * sizeof(Matrix) });
	firstPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/uv_map_ps.hlsl", { sizeof(MaterialCb), 2 * sizeof(Vector4) });

	firstPass.PreRenderCallback = [this, material, &state, &emissive](const BuD::RenderingPass& pass, BuD::SceneEntity entity)
	{
		auto lightProj = BuD::Renderer::ProjectionMatrix();

		m_LightMap = BuD::Renderer::EndTarget();

		auto& faceState = entity.GetComponent<BuD::IState>();

		BuD::Renderer::BeginTarget(2000, 2000);
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

		auto graphicsDevice = BuD::Renderer::Device();
		auto& context = graphicsDevice->Context();

		auto model = dxm::Matrix::CreateFromQuaternion(faceState.Rotation) * dxm::Matrix::CreateRotationX(DirectX::XM_PIDIV2) * dxm::Matrix::CreateTranslation(faceState.Position);
		auto view = entity->ActiveCamera()->ViewMatrix();
		auto proj = BuD::Renderer::ProjectionMatrix();

		Matrix vsCB0[] = { model, view, view.Invert(), proj };
		Matrix vsCB1[] =
		{
			state.Position == faceState.Position
				? dxm::Matrix::Identity
				: dxm::Matrix::CreateLookAt(state.Position, faceState.Position, { 0.0f, 0.0f, 1.0f }),
			lightProj
		};
		pass.VertexShader->UpdateConstantBuffer(0, vsCB0, 4 * sizeof(Matrix));
		pass.VertexShader->UpdateConstantBuffer(1, vsCB1, 2 * sizeof(Matrix));

		MaterialCb psCB0 =
		{
			toV4(material.AmbientColor),
			toV4(material.DiffuseColor),
			toV4(material.SpecularColor),
			material.SpecularExponent,
			material.OpticalDensity,
			material.Dissolve,
			material.Illumination
		};
		Vector4 psCB1[] = { toV4(state.Position), toV4(emissive.Color) };

		pass.PixelShader->UpdateConstantBuffer(0, &psCB0, sizeof(MaterialCb));
		pass.PixelShader->UpdateConstantBuffer(1, psCB1, 2 * sizeof(Vector4));

		ID3D11ShaderResourceView* SRVs[] = { m_DiffuseMap.SRV(), m_AmbientOcclusionMap.SRV(), m_NormalMap.SRV(), m_MicronormalMap.SRV(), m_SpecularMap.SRV(), m_LightMap.SRV()};
		context->PSSetShaderResources(0, 6, SRVs);
	};

	m_RenderingPasses.push_back(firstPass);

	BuD::RenderingPass blurPass{};

	blurPass.VertexBuffer = std::make_shared<BuD::VertexBuffer>(g_QuadVertices.size() * sizeof(Vector2), g_quadLayout, g_QuadVertices.data());
	blurPass.IndexBuffer = std::make_shared<BuD::IndexBuffer>(DXGI_FORMAT_R16_UINT, g_QuadIndices.size() * sizeof(unsigned short), g_QuadIndices.data());

	blurPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/f2_pos_vs.hlsl", g_quadLayout);
	blurPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/gaussianBlur_ps.hlsl");

	blurPass.PreRenderCallback = [this](const BuD::RenderingPass& pass, BuD::SceneEntity entity)
	{
		m_BakedLighting = BuD::Renderer::EndTarget();

		BuD::Renderer::BeginTarget(2000, 2000);
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

		auto srv = m_BakedLighting.SRV();

		auto graphicsDevice = BuD::Renderer::Device();
		auto& context = graphicsDevice->Context();

		context->PSSetShaderResources(0, 1, &srv);
	};

	m_RenderingPasses.push_back(blurPass);

	BuD::RenderingPass finalPass{};

	finalPass.VertexBuffer = m_MeshSegments[0].VertexBuffer;
	finalPass.IndexBuffer = m_MeshSegments[0].IndexBuffer;

	finalPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/objModel_vs.hlsl", m_MeshSegments[0].Layout, { 4 * sizeof(Matrix), 2 * sizeof(dxm::Matrix) });
	finalPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/blurredIrradiance_ps.hlsl", { 2 * sizeof(Matrix) });

	finalPass.PreRenderCallback = [this, &state, &emissive](const BuD::RenderingPass& pass, BuD::SceneEntity entity)
	{
		auto lightProj = BuD::Renderer::ProjectionMatrix();
		m_BlurredBakedIrradiance = BuD::Renderer::EndTarget();

		auto& faceState = entity.GetComponent<BuD::IState>();

		auto model = dxm::Matrix::CreateFromQuaternion(faceState.Rotation) * dxm::Matrix::CreateRotationX(DirectX::XM_PIDIV2) * dxm::Matrix::CreateTranslation(faceState.Position);
		auto view = entity->ActiveCamera()->ViewMatrix();
		auto proj = BuD::Renderer::ProjectionMatrix();

		dxm::Matrix vsCB0[] =
		{
			proj,
			view,
			view.Invert(),
			model,
		};
		dxm::Matrix vsCB1[] =
		{
			lightProj,
			dxm::Matrix::CreateLookAt(state.Position, faceState.Position, { 0.0f, 0.0f, 1.0f })
		};
		dxm::Vector4 psCB0[] = { toV4(state.Position), toV4(emissive.Color) };

		pass.VertexShader->UpdateConstantBuffer(0, vsCB0, 4 * sizeof(Matrix));
		pass.VertexShader->UpdateConstantBuffer(1, vsCB1, 2 * sizeof(Matrix));
		pass.PixelShader->UpdateConstantBuffer(0, psCB0, 2 * sizeof(Matrix));

		ID3D11ShaderResourceView* SRVs[] = { m_BlurredBakedIrradiance.SRV(), m_DiffuseMap.SRV() };

		auto graphicsDevice = BuD::Renderer::Device();
		auto& context = graphicsDevice->Context();

		context->PSSetShaderResources(0, 2, SRVs);
	};

	m_RenderingPasses.push_back(finalPass);

	m_Entity.AddComponent<BuD::IRenderable>(m_RenderingPasses);
	m_Entity.AddComponent<BuD::IState>();
}
