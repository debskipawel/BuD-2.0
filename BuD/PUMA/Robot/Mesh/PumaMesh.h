#pragma once

#include <Robot/Mesh/RobotSegmentMesh.h>

class PumaMesh
{
public:
	PumaMesh(BuD::Scene& scene);

	virtual auto UpdateMesh(std::array<dxm::Vector3, 5> robotJoints) -> void;

protected:
	std::array<std::unique_ptr<RobotSegmentMesh>, 4> m_RobotSegments;
};
