#include "PumaScene.h"

PumaScene::PumaScene()
	: m_Scene()
{
	m_Grid = std::make_unique<Grid>(m_Scene);
	m_RobotMesh = std::make_unique<PumaMesh>(m_Scene);
}

auto PumaScene::UpdateRobotMesh(const RobotParameters& parameters, const RobotConfiguration& robotConfiguration) -> void
{
}

auto PumaScene::Scene() -> BuD::Scene&
{
	return m_Scene;
}
