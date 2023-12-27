#pragma once

#include <BuD.h>

class RobotSegmentMesh
{
public:
	RobotSegmentMesh(BuD::Scene& scene, const dxm::Vector3& position, const dxm::Vector3& orientation, float length, const dxm::Vector3& color);

	virtual auto UpdateInstanceData(const dxm::Vector3& position, const dxm::Vector3& orientation, float length) -> void;
	virtual auto UpdateInstanceData(const dxm::Vector3& position, const dxm::Vector3& orientation, float length, const dxm::Vector3& color) -> void;

protected:
	struct RobotSegmentMeshInstanceData
	{
		dxm::Matrix m_ModelMatrix;
		dxm::Vector3 m_Color;
	};

	RobotSegmentMeshInstanceData m_SegmentInstanceData;

	BuD::SceneEntity m_SegmentEntity;
};
