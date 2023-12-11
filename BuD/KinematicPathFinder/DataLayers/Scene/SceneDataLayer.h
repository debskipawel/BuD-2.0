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
	virtual auto UpdateMeshes(std::pair<float, float> currentConfiguration) -> void;

	virtual auto RecalculateRobotAngleParameterSpace() -> void;
	virtual auto RecalculateRobotPathsInParameterSpace() -> void;
	virtual auto FindPathFromStartingConfiguration() -> std::vector<std::pair<int, int>>;

	BuD::Scene m_Scene;

	ObstacleCollection m_ObstacleCollection;

	BuD::EditableTexture m_AngleParameterMap;

	RobotArmConfiguration m_StartConfiguration;
	RobotArmConfiguration m_EndConfiguration;

protected:
	virtual auto IsCollision(const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2) -> bool;

	float m_Length1;
	float m_Length2;

	std::unique_ptr<RobotArm> m_RobotArm;
	std::unique_ptr<RobotArm> m_StartRobotArm;
	std::unique_ptr<RobotArm> m_EndRobotArm;

	RobotParameterVectorField m_ParameterSpacePathMap;

public:
	constexpr inline virtual auto GetLength1() const noexcept -> decltype(m_Length1) { return m_Length1; }
	constexpr inline virtual auto GetLength2() const noexcept -> decltype(m_Length2) { return m_Length2; }
};
