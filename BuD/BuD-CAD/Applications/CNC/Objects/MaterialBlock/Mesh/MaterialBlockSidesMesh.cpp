#include "MaterialBlockSidesMesh.h"

#include <execution>

MaterialBlockSidesMesh::MaterialBlockSidesMesh(BuD::Scene& scene, std::shared_ptr<BuD::EditableTexture> heightMap, std::shared_ptr<BuD::Texture> surfaceTexture, MaterialBlockParameters blockParameters, dxm::Vector3 position)
	: m_SidesEntity(scene), m_SurfaceTexture(surfaceTexture), m_BlockParameters(blockParameters), m_Position(position)
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
	shaderPipeline.m_VertexShader = BuD::ShaderLoader::VSLoad("Resources/Shaders/MaterialBlock/material_block_sides_vs.hlsl");
	shaderPipeline.m_HullShader = BuD::ShaderLoader::HSLoad("Resources/Shaders/MaterialBlock/material_block_sides_hs.hlsl");
	shaderPipeline.m_DomainShader = BuD::ShaderLoader::DSLoad("Resources/Shaders/MaterialBlock/material_block_sides_ds.hlsl", { 2 * sizeof(dxm::Matrix) });
	shaderPipeline.m_PixelShader = BuD::ShaderLoader::PSLoad("Resources/Shaders/MaterialBlock/material_block_sides_ps.hlsl");

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

		ID3D11ShaderResourceView* dsSRV[] = { m_HeightMap->SRV() };
		ID3D11ShaderResourceView* psSRV[] = { m_SurfaceTexture->SRV() };

		context->DSSetShaderResources(0, 1, dsSRV);
		context->PSSetShaderResources(0, 1, psSRV);
	};

	std::vector<BuD::RenderingPass> renderingPasses = { renderingPass };

	m_SidesEntity.AddComponent<BuD::IRenderable>(renderingPasses);
}

void MaterialBlockSidesMesh::UpdateMaterialBlockParameters(MaterialBlockParameters blockParameters)
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

void MaterialBlockSidesMesh::UpdatePosition(dxm::Vector3 position)
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

void MaterialBlockSidesMesh::UpdateHeightMap(std::shared_ptr<BuD::EditableTexture> heightMap)
{
	constexpr auto TESSELLATION_LIMIT = 64U;

	m_HeightMap = heightMap;

	auto resolutionWidth = m_HeightMap->Width();
	auto resolutionHeight = m_HeightMap->Height();

	auto widthInQuads = static_cast<size_t>(std::ceilf(static_cast<float>(resolutionWidth) / TESSELLATION_LIMIT));
	auto heightInQuads = static_cast<size_t>(std::ceilf(static_cast<float>(resolutionWidth) / TESSELLATION_LIMIT));

	m_InstanceData.clear();
	m_InstanceData.resize(2 * (widthInQuads + heightInQuads));

	std::for_each(std::execution::par, m_InstanceData.begin(), m_InstanceData.end(),
		[this, widthInQuads, heightInQuads, resolutionWidth, resolutionHeight](SingleQuadInstanceData& quad)
		{
			auto idx = &quad - m_InstanceData.data();

			std::array<size_t, 5> ranges = { 0, heightInQuads, heightInQuads + widthInQuads, 2 * heightInQuads + widthInQuads, 2 * (heightInQuads + widthInQuads) };

			auto uvRange = dxm::Vector4();
			auto tessFactor = dxm::Vector2();

			for (size_t i = 0; i < ranges.size() - 1; ++i)
			{
				if (idx < ranges[i] || idx >= ranges[i + 1])
				{
					continue;
				}

				auto wallIndex = idx - ranges[i];
				auto wallCount = ranges[i + 1] - ranges[i];

				auto wallResolution = i % 2 ? resolutionHeight : resolutionWidth;
				auto insignificantParameter = i / 2 ? dxm::Vector2(0.0f, 0.0f) : dxm::Vector2(1.0f, 1.0f);

				auto parameterRangeStart = wallIndex * static_cast<float>(TESSELLATION_LIMIT) / wallResolution;
				auto parameterRangeEnd = std::clamp((wallIndex + 1) * static_cast<float>(TESSELLATION_LIMIT) / wallResolution, 0.0f, 1.0f);

				if (i % 2)
				{
					uvRange.x = insignificantParameter.x;
					uvRange.y = insignificantParameter.y;
					uvRange.z = parameterRangeStart;
					uvRange.w = parameterRangeEnd;
				}
				else
				{
					uvRange.x = parameterRangeStart;
					uvRange.y = parameterRangeEnd;
					uvRange.z = insignificantParameter.x;
					uvRange.w = insignificantParameter.y;
				}

				tessFactor.x = (parameterRangeEnd - parameterRangeStart) * TESSELLATION_LIMIT;
				tessFactor.y = 1.0f;

				break;
			}

			quad.m_RangeUV = uvRange;
			quad.m_TessellationFactor = tessFactor;

			quad.m_Position = m_Position;
			quad.m_MaterialSize = m_BlockParameters.m_Size;
		}
	);
}

void MaterialBlockSidesMesh::UpdateBlock(std::shared_ptr<BuD::EditableTexture> heightMap, MaterialBlockParameters blockParameters, dxm::Vector3 position)
{
	m_BlockParameters = blockParameters;
	m_Position = position;

	UpdateHeightMap(heightMap);
}
