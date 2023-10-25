#include "MaterialBlock.h"

#include <execution>

MaterialBlock::MaterialBlock(BuD::Scene& scene, dxm::Vector3 size, uint32_t resolutionWidth, uint32_t resolutonHeight)
	: m_Position(dxm::Vector3::Zero)
{
	auto albedoMap = BuD::Texture::LoadFromFile("Resources/Textures/wood_light.jpg");
	m_SurfaceTexture = std::make_shared<BuD::Texture>(albedoMap.SRV());

	UpdateResolution(resolutionWidth, resolutonHeight);
	UpdateSize(size);

	m_TopPlane = std::make_unique<MaterialBlockTopMesh>(scene, m_HeightMap, m_SurfaceTexture, m_Size, m_Position);
}

void MaterialBlock::UpdateSize(dxm::Vector3 size)
{
	if (m_Size == size)
	{
		return;
	}

	m_Size = size;

	if (m_TopPlane)
	{
		m_TopPlane->UpdateSize(size);
	}

	auto height = size.y;

	m_HeightMap->BeginEdit();
	m_HeightMap->Clear({ height, height, height, height });
	m_HeightMap->EndEdit();
}

void MaterialBlock::UpdateResolution(uint32_t resolutionWidth, uint32_t resolutionHeight)
{
	if (m_HeightMap && m_HeightMap->Width() == resolutionWidth && m_HeightMap->Height() == resolutionHeight)
	{
		return;
	}

	m_HeightMap = std::make_shared<BuD::EditableTexture>(resolutionWidth, resolutionHeight);

	if (m_TopPlane)
	{
		m_TopPlane->UpdateHeightMap(m_HeightMap);
	}
}
