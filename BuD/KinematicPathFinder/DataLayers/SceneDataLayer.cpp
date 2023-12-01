#include "SceneDataLayer.h"

SceneDataLayer::SceneDataLayer()
	: m_Scene(), m_Obstacles()
{
}

auto SceneDataLayer::AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void
{
	m_Obstacles.push_back(obstacle);
}
