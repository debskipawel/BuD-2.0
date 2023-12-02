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

	dxm::Vector2 m_P0, m_P1, m_P2;
	float m_L1, m_L2;

	std::unique_ptr<RobotArm> m_RobotArm;
	std::vector<std::shared_ptr<Obstacle>> m_Obstacles;
};
