#include "MaterialBlockTopMesh.h"

#include <execution>

MaterialBlockTopMesh::MaterialBlockTopMesh(BuD::Scene& scene, std::shared_ptr<BuD::EditableTexture> heightMap, std::shared_ptr<BuD::Texture> surfaceTexture, MaterialBlockParameters blockParameters, dxm::Vector3 position)
	: m_TopEntity(scene), m_SurfaceTexture(surfaceTexture), m_BlockParameters(blockParameters), m_Position(position)
{
	UpdateHeightMap(heightMap);

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
	renderingPass.m_RasterizerDescription.m_CullType = BuD::CullType::BACK;
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

		ID3D11ShaderResourceView* dsSRV[] = { m_HeightMap->SRV() };
		ID3D11ShaderResourceView* psSRV[] = { m_SurfaceTexture->SRV() };

		context->DSSetShaderResources(0, 1, dsSRV);
		context->PSSetShaderResources(0, 1, psSRV);
	};

	std::vector<BuD::RenderingPass> renderingPasses = { renderingPass };

	m_TopEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void MaterialBlockTopMesh::UpdateMaterialBlockParameters(MaterialBlockParameters blockParameters)
{
	if (m_BlockParameters == blockParameters)
	{
		return;
	}

	m_BlockParameters = blockParameters;

	std::for_each(std::execution::par, m_InstanceData.begin(), m_InstanceData.end(),
		[&blockParameters](SingleQuadInstanceData& data)
		{
			data.m_MaterialSize = blockParameters.m_Size;
		}
	);
}

void MaterialBlockTopMesh::UpdatePosition(dxm::Vector3 position)
{
	if (m_Position == position)
	{
		return;
	}

	m_Position = position;

	std::for_each(std::execution::par, m_InstanceData.begin(), m_InstanceData.end(),
		[&position](SingleQuadInstanceData& data)
		{
			data.m_Position = position;
		}
	);
}

void MaterialBlockTopMesh::UpdateHeightMap(std::shared_ptr<BuD::EditableTexture> heightMap)
{
	constexpr auto TESSELLATION_LIMIT = 64U;

	m_HeightMap = heightMap;

	auto resolutionWidth = m_HeightMap->Width();
	auto resolutionHeight = m_HeightMap->Height();

	auto widthInQuads = static_cast<size_t>(std::ceilf(static_cast<float>(resolutionWidth) / TESSELLATION_LIMIT));
	auto heightInQuads = static_cast<size_t>(std::ceilf(static_cast<float>(resolutionWidth) / TESSELLATION_LIMIT));

	m_InstanceData.clear();
	m_InstanceData.resize(widthInQuads * heightInQuads);

	std::for_each(std::execution::par, m_InstanceData.begin(), m_InstanceData.end(),
		[this, widthInQuads, heightInQuads, resolutionWidth, resolutionHeight](SingleQuadInstanceData& quad)
		{
			auto idx = &quad - m_InstanceData.data();

			auto col = idx % widthInQuads;
			auto row = idx / widthInQuads;

			quad.m_RangeUV.x = col * static_cast<float>(TESSELLATION_LIMIT) / resolutionWidth;
			quad.m_RangeUV.y = std::clamp((col + 1) * static_cast<float>(TESSELLATION_LIMIT) / resolutionWidth, 0.0f, 1.0f);
			quad.m_RangeUV.z = row * static_cast<float>(TESSELLATION_LIMIT) / resolutionHeight;
			quad.m_RangeUV.w = std::clamp((row + 1) * static_cast<float>(TESSELLATION_LIMIT) / resolutionHeight, 0.0f, 1.0f);

			quad.m_TessellationFactor.x = min(resolutionWidth - col * TESSELLATION_LIMIT, TESSELLATION_LIMIT);
			quad.m_TessellationFactor.y = min(resolutionHeight - row * TESSELLATION_LIMIT, TESSELLATION_LIMIT);

			quad.m_Position = m_Position;
			quad.m_MaterialSize = m_BlockParameters.m_Size;
		}
	);
}

void MaterialBlockTopMesh::UpdateBlock(std::shared_ptr<BuD::EditableTexture> heightMap, MaterialBlockParameters blockParameters, dxm::Vector3 position)
{
	m_BlockParameters = blockParameters;
	m_Position = position;

	UpdateHeightMap(heightMap);
}
