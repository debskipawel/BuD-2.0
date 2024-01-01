#include "PumaScene.h"

#include <Robot/Calculators/AllJointFramesCalculator.h>

PumaScene::PumaScene()
	: m_Scene()
{
	m_Grid = std::make_unique<Grid>(m_Scene);
	m_RobotMesh = std::make_unique<PumaMesh>(m_Scene);
}

auto PumaScene::UpdateRobotMesh(const RobotParameters& parameters, const RobotConfiguration& robotConfiguration) -> void
{
	auto allJointsCalculator = AllJointFramesCalculator();

	auto [F0, F1, F2, F3, F4, F5] = allJointsCalculator.Calculate(robotConfiguration, parameters);

	m_RobotMesh->UpdateMesh({ F1.Position(), F2.Position(), F3.Position(), F4.Position(), F5.Position() });
}

auto PumaScene::Scene() -> BuD::Scene&
{
	return m_Scene;
}
