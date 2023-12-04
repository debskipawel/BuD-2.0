#pragma once

#include <DataLayers/Scene/ObstacleCollection.h>
#include <Scene/RobotArmConfiguration.h>

class RobotParameterSpaceDrawer
{
public:
	RobotParameterSpaceDrawer(ObstacleCollection& obstacleCollection);

	virtual auto DrawRobotParameterSpace(const dxm::Vector2& p0, float L1, float L2, BuD::EditableTexture& parameterSpace) -> void;

protected:
	virtual auto GetCollidingObstacle(const dxm::Vector2& p0, const dxm::Vector2& p1) -> std::shared_ptr<Obstacle>;

	ObstacleCollection& m_ObstacleCollection;
};
