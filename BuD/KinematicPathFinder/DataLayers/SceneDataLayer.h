#pragma once

#include <BuD.h>

#include <Scene/Obstacle.h>
#include <Scene/RobotArm.h>

struct SceneDataLayer
{
public:
	SceneDataLayer();

	virtual auto AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void;

	BuD::Scene m_Scene;

	std::unique_ptr<RobotArm> m_RobotArm;
	std::vector<std::shared_ptr<Obstacle>> m_Obstacles;
};
