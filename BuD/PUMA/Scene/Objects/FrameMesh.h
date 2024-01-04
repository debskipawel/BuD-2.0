#pragma once

#include <BuD.h>

#include <Robot/Frame.h>

class FrameMesh
{
public:
	FrameMesh(BuD::Scene& scene, const Frame& frame);

	virtual auto UpdateMesh(const Frame& frame) -> void;

protected:
	struct FrameArrowInstanceData 
	{
		dxm::Matrix m_ModelMatrix;
		dxm::Vector4 m_Color;
	};

	std::array<FrameArrowInstanceData, 3> m_InstanceData;
	dxm::Vector3 m_FramePosition;

	BuD::SceneEntity m_FrameEntity;
};
