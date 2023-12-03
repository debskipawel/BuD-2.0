#pragma once

#include <BuD.h>

#include <Scene/Obstacle.h>
#include <Scene/RobotArm.h>

#include <Scene/RobotArmConfiguration.h>

struct SceneDataLayer
{
public:
	SceneDataLayer();

	virtual auto AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void;
	virtual auto RemoveObstacle(size_t idx) -> void;
	
	virtual auto UpdateStartConfigurationPoints(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void;
	virtual auto UpdateEndConfigurationPoints(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void;
	virtual auto UpdateConfigurationLength(float L1, float L2) -> void;

	virtual auto UpdateMeshes() -> void;

	BuD::Scene m_Scene;

	BuD::EditableTexture m_AngleParameterMap;

	RobotArmConfiguration m_StartConfiguration;
	RobotArmConfiguration m_EndConfiguration;

	std::unique_ptr<RobotArm> m_RobotArm;
	std::unique_ptr<RobotArm> m_StartRobotArm;
	std::unique_ptr<RobotArm> m_EndRobotArm;

	std::vector<std::shared_ptr<Obstacle>> m_Obstacles;

protected:
	virtual auto GetCollidingObstacle(const dxm::Vector2& p1, const dxm::Vector2& p2) -> std::shared_ptr<Obstacle>;
	
	virtual auto IsCollision(const dxm::Vector2& p1, const dxm::Vector2& p2, const Obstacle& obstacle) -> bool;
	virtual auto IsCollision(const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2) -> bool;

	virtual auto UpdateAngleParameterMap() -> void;
};
