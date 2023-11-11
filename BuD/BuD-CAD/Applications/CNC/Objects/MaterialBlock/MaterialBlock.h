#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>

#include <Applications/CNC/Objects/MaterialBlock/Mesh/MaterialBlockTopMesh.h>
#include <Applications/CNC/Objects/MaterialBlock/Mesh/MaterialBlockSidesMesh.h>

class MaterialBlock
{
public:
	MaterialBlock(BuD::Scene& scene, const MaterialBlockParameters& blockParameters);

	void UpdateParameters(const MaterialBlockParameters& blockParameters);

	std::shared_ptr<BuD::EditableTexture> HeightMap() const;
	
	dxm::Vector3 Position() const;
	dxm::Vector3 Size() const;

protected:
	MaterialBlockParameters m_BlockParameters;

	std::shared_ptr<BuD::Texture> m_SurfaceTexture;
	std::shared_ptr<BuD::EditableTexture> m_HeightMap;

	std::unique_ptr<MaterialBlockTopMesh> m_TopPlane;
	std::unique_ptr<MaterialBlockSidesMesh> m_SidesPlane;
};
