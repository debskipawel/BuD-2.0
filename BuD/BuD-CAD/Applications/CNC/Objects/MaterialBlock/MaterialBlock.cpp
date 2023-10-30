#include "MaterialBlock.h"

#include <execution>

MaterialBlock::MaterialBlock(BuD::Scene& scene, const MaterialBlockParameters& blockParameters, uint32_t resolutionWidth, uint32_t resolutonHeight)
	: m_Position(dxm::Vector3::Zero), m_BlockParameters(blockParameters)
{
	auto albedoMap = BuD::Texture::LoadFromFile("Resources/Textures/wood_light.jpg");
	m_SurfaceTexture = std::make_shared<BuD::Texture>(albedoMap.SRV());

	UpdateParameters(blockParameters, resolutionWidth, resolutonHeight);

	m_TopPlane = std::make_unique<MaterialBlockTopMesh>(scene, m_HeightMap, m_SurfaceTexture, blockParameters, m_Position);
	m_SidesPlane = std::make_unique<MaterialBlockSidesMesh>(scene, m_HeightMap, m_SurfaceTexture, blockParameters, m_Position);
}

void MaterialBlock::UpdateParameters(const MaterialBlockParameters& blockParameters, uint32_t resolutionWidth, uint32_t resolutionHeight)
{
	auto modified = (m_BlockParameters == blockParameters);
	m_BlockParameters = blockParameters;

	if (!m_HeightMap || m_HeightMap->Width() != resolutionWidth || m_HeightMap->Height() != resolutionHeight)
	{
		modified = true;
		m_HeightMap = std::make_shared<BuD::EditableTexture>(resolutionWidth, resolutionHeight);
	}

	if (!modified)
	{
		return;
	}

	auto height = m_BlockParameters.m_Size.y;

	m_HeightMap->BeginEdit();
	m_HeightMap->Clear({ height, height, height, height });
	m_HeightMap->EndEdit();

	if (m_TopPlane)
	{
		m_TopPlane->UpdateBlock(m_HeightMap, blockParameters, m_Position);
	}

	if (m_SidesPlane)
	{
		m_SidesPlane->UpdateBlock(m_HeightMap, blockParameters, m_Position);
	}
}

std::shared_ptr<BuD::EditableTexture> MaterialBlock::HeightMap() const
{
	return m_HeightMap;
}

dxm::Vector3 MaterialBlock::Position() const
{
	return m_Position;
}

dxm::Vector3 MaterialBlock::Size() const
{
	return m_BlockParameters.m_Size;
}
