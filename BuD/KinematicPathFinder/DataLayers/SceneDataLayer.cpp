#include "SceneDataLayer.h"

SceneDataLayer::SceneDataLayer()
	: m_Scene(), m_Obstacles(), m_L1(0.4f), m_L2(0.25f)
{
	m_P0 = dxm::Vector2::Zero;
	m_P1 = m_P0 + m_L1 * dxm::Vector2::UnitX;
	m_P2 = m_P1 + m_L2 * dxm::Vector2::UnitY;

	m_RobotArm = std::make_unique<RobotArm>(m_Scene, m_P0, m_P1, m_P2);
}

auto SceneDataLayer::AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void
{
	m_Obstacles.push_back(obstacle);
}
