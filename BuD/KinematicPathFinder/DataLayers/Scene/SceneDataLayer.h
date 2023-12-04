#pragma once

#include <BuD.h>

#include <DataLayers/Scene/ObstacleCollection.h>
#include <DataLayers/Scene/RobotParameterSpaceVectorFieldCalculator.h>

#include <Scene/Obstacle.h>
#include <Scene/RobotArm.h>

#include <Scene/RobotArmConfiguration.h>

struct SceneDataLayer
{
public:
	SceneDataLayer();

	virtual auto AddNewObstacle() -> void;
	
	virtual auto UpdateStartConfigurationPoints(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void;
	virtual auto UpdateEndConfigurationPoints(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void;
	virtual auto UpdateConfigurationLength(float L1, float L2) -> void;

	virtual auto UpdateMeshes() -> void;

	virtual auto RecalculateRobotAngleParameterSpace() -> void;
	virtual auto RecalculateRobotPathsInParameterSpace() -> void;
	virtual auto FindPathFromStartingConfiguration() -> std::vector<std::pair<int, int>>;

	BuD::Scene m_Scene;

	ObstacleCollection m_ObstacleCollection;

	BuD::EditableTexture m_AngleParameterMap;

	RobotArmConfiguration m_StartConfiguration;
	RobotArmConfiguration m_EndConfiguration;

	std::unique_ptr<RobotArm> m_RobotArm;
	std::unique_ptr<RobotArm> m_StartRobotArm;
	std::unique_ptr<RobotArm> m_EndRobotArm;

protected:
	virtual auto PointInParameterSpace(const RobotArmConfiguration& configuration) -> std::pair<int, int>;

	virtual auto IsCollision(const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2) -> bool;

	std::map<std::pair<int, int>, VectorFieldPoint> m_ParameterSpacePathMap;
};
