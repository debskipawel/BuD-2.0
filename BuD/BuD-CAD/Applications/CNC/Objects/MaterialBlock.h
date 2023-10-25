#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockTopMesh.h>

class MaterialBlock
{
public:
	MaterialBlock(BuD::Scene& scene, dxm::Vector3 size, uint32_t resolutionWidth, uint32_t resolutonHeight);

	void UpdateSize(dxm::Vector3 size);
	void UpdateResolution(uint32_t resolutionWidth, uint32_t resolutionHeight);

protected:

	dxm::Vector3 m_Size;
	dxm::Vector3 m_Position;

	std::shared_ptr<BuD::Texture> m_SurfaceTexture;
	std::shared_ptr<BuD::EditableTexture> m_HeightMap;

	std::unique_ptr<MaterialBlockTopMesh> m_TopPlane;
};
