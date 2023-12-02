#include "SceneDataLayer.h"

SceneDataLayer::SceneDataLayer()
	: m_Scene(), m_Obstacles()
{
	m_RobotArm = std::make_unique<RobotArm>(m_Scene, dxm::Vector2(0.0f, 0.0f), dxm::Vector2(1.0f, 0.1f), dxm::Vector2(1.1f, 1.1f));
}

auto SceneDataLayer::AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void
{
	m_Obstacles.push_back(obstacle);
}
