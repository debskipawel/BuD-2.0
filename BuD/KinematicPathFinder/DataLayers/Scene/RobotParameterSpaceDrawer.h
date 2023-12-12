#pragma once

#include <DataLayers/Scene/ObstacleCollection.h>
#include <DataLayers/Scene/RobotParameterVectorField.h>
#include <Scene/RobotArmConfiguration.h>

class RobotParameterSpaceDrawer
{
public:
	RobotParameterSpaceDrawer(ObstacleCollection& obstacleCollection);

	virtual auto DrawRobotParameterSpace(const dxm::Vector2& p0, float L1, float L2, BuD::EditableTexture& parameterSpace, const RobotParameterVectorField& vectorField) -> void;

protected:
	virtual auto GetCollidingObstacle(const dxm::Vector2& p0, const dxm::Vector2& p1) -> std::shared_ptr<Obstacle>;
	
	virtual auto DrawObstacles(const dxm::Vector2& p0, float L1, float L2, BuD::EditableTexture& parameterSpace) -> void;
	virtual auto DrawDistanceGradient(BuD::EditableTexture& parameterSpace, const RobotParameterVectorField& vectorField) -> void;

	ObstacleCollection& m_ObstacleCollection;
};
