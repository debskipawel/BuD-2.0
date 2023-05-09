#include "DepthMapFaceMesh.h"

#include <array>

#include <Objects/Interfaces/IEmissive.h>
#include <Objects/MeshLoader/MeshLoader.h>

#include <Shaders/Loader/ShaderLoader.h>

struct DepthStageCb
{
	dxm::Matrix proj;
	dxm::Matrix view;
	dxm::Matrix model;
	float grow;
	dxm::Vector3 __fill;
};

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

dxm::Vector4 v4(const dxm::Vector3& other)
{
	return { other.x, other.y, other.z, 0.0f };
}

DepthMapFaceMesh::DepthMapFaceMesh(BuD::Scene& scene)
	: FaceMesh(scene), m_Grow(0.0f)
{
	auto material = m_MeshSegments[0].Material;
	
	auto [entity, emissive, state] = *scene.GetAllEntitiesWith<BuD::IEmissive, BuD::IState>().begin();

	BuD::RenderingPass renderPass{};

	renderPass.VertexBuffer = m_MeshSegments[0].VertexBuffer;
	renderPass.IndexBuffer = m_MeshSegments[0].IndexBuffer;

	renderPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/depthMapAbsorption_vs.hlsl", m_MeshSegments[0].Layout, { sizeof(DepthStageCb) });
	renderPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/depthMapAbsorption_ps.hlsl");

	renderPass.PreRenderCallback = [this, &state](const BuD::RenderingPass& renderPass, BuD::SceneEntity entity)
	{
		BuD::Renderer::BeginTarget(4000, 4000);
		BuD::Renderer::Clear(0.0f, 0.0f, 0.0f, 1.0f);

		auto& faceState = entity.GetComponent<BuD::IState>();

		auto model = dxm::Matrix::CreateFromQuaternion(faceState.Rotation) * dxm::Matrix::CreateRotationX(DirectX::XM_PIDIV2) * dxm::Matrix::CreateTranslation(faceState.Position);
		auto view = state.Position == faceState.Position 
			? dxm::Matrix::Identity 
			: dxm::Matrix::CreateLookAt(state.Position, faceState.Position, { 0.0f, 0.0f, 1.0f });
		auto proj = BuD::Renderer::ProjectionMatrix();

		DepthStageCb cb =
		{
			proj,
			view,
			model,
			m_Grow
		};

		renderPass.VertexShader->UpdateConstantBuffer(0, &cb, sizeof(DepthStageCb));
	};

	m_RenderingPasses.push_back(renderPass);

	BuD::RenderingPass finalRenderPass{};

	finalRenderPass.VertexBuffer = m_MeshSegments[0].VertexBuffer;
	finalRenderPass.IndexBuffer = m_MeshSegments[0].IndexBuffer;

	finalRenderPass.VertexShader = BuD::ShaderLoader::VSLoad(L"../x64/Debug/objModel_vs.hlsl", m_MeshSegments[0].Layout, { sizeof(DepthStageCb), 2 * sizeof(dxm::Matrix) });
	finalRenderPass.PixelShader = BuD::ShaderLoader::PSLoad(L"../x64/Debug/objModel_ps.hlsl", { 10 * sizeof(int), sizeof(MaterialCb), 2 * sizeof(dxm::Vector4) });

	finalRenderPass.PreRenderCallback = [this, material, &state, &emissive](const BuD::RenderingPass& renderPass, BuD::SceneEntity entity)
	{
		auto lightProj = BuD::Renderer::ProjectionMatrix();
		
		m_LightDepthMap = BuD::Renderer::EndTarget();

		// set all the textures
		std::array<ID3D11ShaderResourceView*, 10> depthMapSrv = 
		{
			m_TransmissionMap.SRV(),
			m_SpecularMap.SRV(),
			m_SSSMap.SRV(),
			m_RoughnessMap.SRV(),
			m_NormalMap.SRV(),
			m_MicronormalMask.SRV(),
			m_MicronormalMap.SRV(),
			m_DiffuseMap.SRV(),
			m_AmbientOcclusionMap.SRV(),
			m_LightDepthMap.SRV(),
		};

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
			state.Position == faceState.Position
				? dxm::Matrix::Identity
				: dxm::Matrix::CreateLookAt(state.Position, faceState.Position, { 0.0f, 0.0f, 1.0f })
		};

		int psCB0[] = {
			m_TransmissionMapOn, m_SpecularMapOn, m_SSSMapOn, m_RoughnessMapOn, m_NormalMapOn,
			m_MicronormalMaskOn, m_MicronormalMapOn, m_DiffuseMapOn, m_AmbientOcclussionMapOn, m_DepthMapAbsorptionOn
		};
		MaterialCb psCB1 = 
		{ 
			v4(material.AmbientColor),
			v4(material.DiffuseColor),
			v4(material.SpecularColor),
			material.SpecularExponent,
			material.OpticalDensity,
			material.Dissolve,
			material.Illumination,
			v4(m_PassingExpMultiplier)
		};
		dxm::Vector4 psCB2[] = { v4(state.Position), v4(emissive.Color) };

		auto& context = BuD::Renderer::Device()->Context();
		context->PSSetShaderResources(0, depthMapSrv.size(), depthMapSrv.data());

		renderPass.VertexShader->UpdateConstantBuffer(0, vsCB0, 4 * sizeof(dxm::Matrix));
		renderPass.VertexShader->UpdateConstantBuffer(1, vsCB1, 2 * sizeof(dxm::Matrix));
		renderPass.PixelShader->UpdateConstantBuffer(0, psCB0, 10 * sizeof(int));
		renderPass.PixelShader->UpdateConstantBuffer(1, &psCB1, sizeof(MaterialCb));
		renderPass.PixelShader->UpdateConstantBuffer(2, psCB2, 2 * sizeof(dxm::Vector4));
	};

	m_RenderingPasses.push_back(finalRenderPass);

	m_Entity.AddComponent<BuD::IRenderable>(m_RenderingPasses);
	m_Entity.AddComponent<BuD::IState>();
}
