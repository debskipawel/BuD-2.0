#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/MaterialBlock/MaterialBlockParameters.h>

class MaterialBlockSidesMesh
{
public:
	MaterialBlockSidesMesh(BuD::Scene& scene, std::shared_ptr<BuD::EditableTexture> heightMap, std::shared_ptr<BuD::Texture> surfaceTexture, MaterialBlockParameters blockParameters, dxm::Vector3 position);

	virtual void UpdateMaterialBlockParameters(MaterialBlockParameters blockParameters);
	virtual void UpdatePosition(dxm::Vector3 position);
	virtual void UpdateHeightMap(std::shared_ptr<BuD::EditableTexture> heightMap);

	virtual void UpdateBlock(std::shared_ptr<BuD::EditableTexture> heightMap, MaterialBlockParameters blockParameters, dxm::Vector3 position);

protected:
	struct SingleQuadInstanceData
	{
		dxm::Vector4 m_RangeUV;
		dxm::Vector2 m_TessellationFactor;
		dxm::Vector3 m_MaterialSize;
		dxm::Vector3 m_Position;
	};

	std::vector<SingleQuadInstanceData> m_InstanceData;

	BuD::SceneEntity m_SidesEntity;

	std::shared_ptr<BuD::EditableTexture> m_HeightMap;
	std::shared_ptr<BuD::Texture> m_SurfaceTexture;

	MaterialBlockParameters m_BlockParameters;
	dxm::Vector3 m_Position;
};
