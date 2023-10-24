#pragma once

#include <BuD.h>

class MaterialBlock
{
public:
	MaterialBlock(BuD::Scene& scene, dxm::Vector3 size, uint32_t resolutionWidth, uint32_t resolutonHeight);

	void SetResolution(uint32_t resolutionWidth, uint32_t resolutionHeight);

protected:
	virtual BuD::RenderingPass GetHeightMapRenderingPass();

	virtual void SetupMesh();

	struct SingleQuadInstanceData
	{
		dxm::Vector4 m_RangeUV;
		dxm::Vector2 m_TessellationFactor;
		dxm::Vector3 m_MaterialSize;
		dxm::Vector3 m_Position;
	};

	std::vector<SingleQuadInstanceData> m_InstanceData;

	dxm::Vector3 m_Size;
	uint32_t m_ResolutionWidth, m_ResolutionHeight;

	BuD::SceneEntity m_Entity;

	BuD::Texture m_SurfaceTexture;
	BuD::EditableTexture m_HeightMap;
};
