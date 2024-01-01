#pragma once

#include <BuD.h>

#include <DataLayers/Camera/CameraDataLayer.h>
#include <DataLayers/Simulation/BaseSimulation.h>

#include <Robot/RobotParameters.h>
#include <Robot/Animation/AnimationClip.h>

struct MainDataLayer
{
public:
	MainDataLayer();

	virtual auto Update(float deltaTime) -> void;

	virtual auto Start() -> void;
	virtual auto Stop() -> void;

	virtual auto ToggleLoop() -> void;

	virtual auto IsRunning() const -> bool;
	virtual auto IsLooped() const -> bool;

	virtual auto GetSimulationTime() const -> float;
	virtual auto SetSimulationTime(float simulationTime) -> void;

	virtual auto RevalidateSimulationMeshes() -> void;

	CameraDataLayer m_CameraDataLayer;
	RobotParameters m_RobotParameters;
	
	AnimationClip m_AnimationClip;

	std::vector<std::shared_ptr<BaseSimulation>> m_Simulations;

protected:
	float m_SimulationTime;

	bool m_Looped;
	bool m_Running;
};
