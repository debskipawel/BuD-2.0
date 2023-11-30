#pragma once

#include <BuD.h>

#include <Scene/Obstacle.h>

class KinematicPathScene
{
public:
	KinematicPathScene();

	BuD::Scene m_Scene;

	std::vector<std::shared_ptr<Obstacle>> m_Obstacles;
};
