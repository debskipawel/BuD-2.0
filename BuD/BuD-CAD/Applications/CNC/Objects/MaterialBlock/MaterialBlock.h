#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>
#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockTopMesh.h>

class MaterialBlock
{
public:
	MaterialBlock(BuD::Scene& scene, const MaterialBlockParameters& blockParameters, uint32_t resolutionWidth, uint32_t resolutonHeight);

	void UpdateParameters(const MaterialBlockParameters& blockParameters, uint32_t resolutionWidth, uint32_t resolutionHeight);

protected:
	MaterialBlockParameters m_BlockParameters;
	dxm::Vector3 m_Position;

	std::shared_ptr<BuD::Texture> m_SurfaceTexture;
	std::shared_ptr<BuD::EditableTexture> m_HeightMap;

	std::unique_ptr<MaterialBlockTopMesh> m_TopPlane;
};
