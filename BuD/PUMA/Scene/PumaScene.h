#pragma once

#include <BuD.h>

#include <Robot/RobotParameters.h>
#include <Robot/RobotConfiguration.h>
#include <Robot/Mesh/PumaMesh.h>

#include <Scene/Objects/FrameMesh.h>
#include <Scene/Objects/Grid.h>

class PumaScene
{
public:
	PumaScene();

	virtual auto UpdateRobotMesh(const RobotParameters& parameters, const RobotConfiguration& robotConfiguration) -> void;
	virtual auto UpdateFrameMesh(const Frame& frame) -> void;

	virtual auto Scene() -> BuD::Scene&;

protected:
	BuD::Scene m_Scene;

	std::unique_ptr<Grid> m_Grid;
	std::unique_ptr<FrameMesh> m_FrameMesh;
	std::unique_ptr<PumaMesh> m_RobotMesh;
};
