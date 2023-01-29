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
	
	BuD::RenderingPass firstPass{};

	firstPass.VertexBuffer = m_MeshSegments[0].VertexBuffer;
	firstPass.IndexBuffer = m_MeshSegments[0].IndexBuffer;

	firstPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/uv_map_vs.hlsl", m_MeshSegments[0].Layout, { 4 * sizeof(Matrix) });
	firstPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/uv_map_ps.hlsl", { sizeof(MaterialCb), 2 * sizeof(Vector4) });

	firstPass.PreRenderCallback = [this, material, &state, &emissive](const BuD::RenderingPass& pass, BuD::SceneEntity entity)
	{
		auto& faceState = entity.GetComponent<BuD::IState>();

		BuD::Renderer::BeginTarget(2000, 2000);
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

		auto graphicsDevice = BuD::Renderer::Device();
		auto& context = graphicsDevice->Context();

		auto model = dxm::Matrix::CreateFromQuaternion(faceState.Rotation) * dxm::Matrix::CreateRotationX(DirectX::XM_PIDIV2) * dxm::Matrix::CreateTranslation(faceState.Position);
		auto view = state.Position == faceState.Position
			? dxm::Matrix::Identity
			: dxm::Matrix::CreateLookAt(state.Position, faceState.Position, { 0.0f, 0.0f, 1.0f });
		auto proj = BuD::Renderer::ProjectionMatrix();

		Matrix vsCB0[] = { model, view, view.Invert(), proj };
		pass.VertexShader->UpdateConstantBuffer(0, vsCB0, 4 * sizeof(Matrix));

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

		ID3D11ShaderResourceView* SRVs[] = { m_DiffuseMap.SRV(), m_AmbientOcclusionMap.SRV(), m_NormalMap.SRV(), m_MicronormalMap.SRV(), m_SpecularMap.SRV() };
		context->PSSetShaderResources(0, 5, SRVs);
	};

	m_RenderingPasses.push_back(firstPass);

	m_Entity.AddComponent<BuD::IRenderable>(m_RenderingPasses);
	m_Entity.AddComponent<BuD::IState>();
}
