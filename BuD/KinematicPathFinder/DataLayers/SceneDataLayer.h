#pragma once

#include <BuD.h>
#include <Scene/Obstacle.h>

struct SceneDataLayer
{
public:
	SceneDataLayer();

	BuD::Scene m_Scene;

	std::vector<std::shared_ptr<Obstacle>> m_Obstacles;
};
