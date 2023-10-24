#include "MaterialBlock.h"

MaterialBlock::MaterialBlock(BuD::Scene& scene, dxm::Vector3 size, uint32_t resolutionWidth, uint32_t resolutonHeight)
	: m_Entity(scene), m_Size(size), m_ResolutionWidth(resolutionWidth), m_ResolutionHeight(resolutonHeight), m_HeightMap(resolutionWidth, resolutonHeight)
{
	m_SurfaceTexture = BuD::Texture::LoadFromFile("Resources/Textures/wood_light.jpg");

	m_HeightMap.BeginEdit();
	m_HeightMap.FloodFill(0.0f, 0.0f, { 0.1f, 0.0f, 0.0f, 0.0f });
	m_HeightMap.EndEdit();

	SetupMesh();

	auto heightMapRenderingPass = GetHeightMapRenderingPass();

	std::vector<BuD::RenderingPass> renderingPasses = { heightMapRenderingPass };

	m_Entity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void MaterialBlock::SetResolution(uint32_t resolutionWidth, uint32_t resolutionHeight)
{
	m_ResolutionWidth = resolutionWidth;
	m_ResolutionHeight = resolutionHeight;

	m_HeightMap = BuD::EditableTexture(resolutionWidth, resolutionHeight);

	SetupMesh();
}

BuD::RenderingPass MaterialBlock::GetHeightMapRenderingPass()
{
	auto meshLoader = BuD::MeshLoader();
	auto mesh = meshLoader.LoadPrimitiveMesh(
		BuD::MeshPrimitiveType::QUAD_TESSELLATION,
		{
			{ "UV_RANGE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "TESSFACTOR", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 4 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "MATERIAL_SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 6 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "MATERIAL_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 9 * sizeof(float), D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		}
	);

	auto shaderPipeline = BuD::ShaderPipeline();
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/MaterialBlock/material_block_vs.hlsl");
	shaderPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("Resources/Shaders/MaterialBlock/material_block_hs.hlsl");
	shaderPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("Resources/Shaders/MaterialBlock/material_block_ds.hlsl", { 2 * sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/MaterialBlock/material_block_ps.hlsl");

	auto instancing = BuD::InstanceComponent();
	instancing.m_InstanceCallback = [this]()
	{
		BuD::InstanceData data;

		data.m_Data = reinterpret_cast<uint8_t*>(m_InstanceData.data());
		data.m_Size = m_InstanceData.size() * sizeof(SingleQuadInstanceData);
		data.m_InstancesCount = m_InstanceData.size();

		return data;
	};

	auto renderingPass = BuD::RenderingPass();
	renderingPass.m_Mesh = mesh;
	renderingPass.m_Pipeline = shaderPipeline;
	renderingPass.m_Instancing = instancing;
	renderingPass.m_RasterizerDescription.m_FillMode = BuD::FillMode::SOLID;
	renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::NONE;
	renderingPass.m_PreRenderCallback = [this](const BuD::RenderingPass& renderingPass, const BuD::Scene& scene)
	{
		auto camera = scene.ActiveCamera();

		auto viewMtx = camera->ViewMatrix();
		auto projMtx = BuD::Renderer::ProjectionMatrix();

		dxm::Matrix cbDS[] = { viewMtx, projMtx };

		auto& ds = renderingPass.m_Pipeline.m_DomainShader;
		ds->UpdateConstantBuffer(0, cbDS, 2 * sizeof(dxm::Matrix));

		auto device = BuD::Renderer::Device();
		auto context = device->Context();

		ID3D11ShaderResourceView* dsSRV[] = { m_HeightMap.SRV() };
		ID3D11ShaderResourceView* psSRV[] = { m_SurfaceTexture.SRV() };

		context->DSSetShaderResources(0, 1, dsSRV);
		context->PSSetShaderResources(0, 1, psSRV);
	};

	return renderingPass;
}

void MaterialBlock::SetupMesh()
{
	constexpr auto TESSELLATION_LIMIT = 64U;

	auto widthInQuads = static_cast<size_t>(std::ceilf(static_cast<float>(m_ResolutionWidth) / TESSELLATION_LIMIT));
	auto heightInQuads = static_cast<size_t>(std::ceilf(static_cast<float>(m_ResolutionHeight) / TESSELLATION_LIMIT));

	m_InstanceData.clear();

	for (size_t row = 0; row < heightInQuads; row++)
	{
		for (size_t col = 0; col < widthInQuads; col++)
		{
			auto quad = SingleQuadInstanceData();

			quad.m_RangeUV.x = col * static_cast<float>(TESSELLATION_LIMIT) / m_ResolutionWidth;
			quad.m_RangeUV.y = std::clamp((col + 1) * static_cast<float>(TESSELLATION_LIMIT) / m_ResolutionWidth, 0.0f, 1.0f);
			quad.m_RangeUV.z = row * static_cast<float>(TESSELLATION_LIMIT) / m_ResolutionHeight;
			quad.m_RangeUV.w = std::clamp((row + 1) * static_cast<float>(TESSELLATION_LIMIT) / m_ResolutionHeight, 0.0f, 1.0f);

			quad.m_TessellationFactor.x = min(m_ResolutionWidth - col * TESSELLATION_LIMIT, TESSELLATION_LIMIT);
			quad.m_TessellationFactor.y = min(m_ResolutionHeight - row * TESSELLATION_LIMIT, TESSELLATION_LIMIT);

			quad.m_Position = dxm::Vector3::Zero;
			quad.m_MaterialSize = m_Size;

			m_InstanceData.push_back(quad);
		}
	}
}
