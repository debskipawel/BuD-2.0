#pragma once

#include <BuD.h>
#include <Scene/Obstacle.h>

struct SceneDataLayer
{
public:
	SceneDataLayer();

	virtual auto AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void;

	BuD::Scene m_Scene;

	std::vector<std::shared_ptr<Obstacle>> m_Obstacles;
};
