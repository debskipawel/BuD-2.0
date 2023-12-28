#pragma once

#include <BuD.h>

#include <DataLayers/Camera/CameraDataLayer.h>
#include <DataLayers/Simulation/SimulationDataLayer.h>

#include <Robot/RobotParameters.h>
#include <Robot/Animation/AnimationClip.h>

struct MainDataLayer
{
public:
	MainDataLayer();

	virtual auto Update(float deltaTime) -> void;

	virtual auto Start() -> void;
	virtual auto Stop() -> void;

	virtual auto IsRunning() const -> bool;

	CameraDataLayer m_CameraDataLayer;

	RobotParameters m_RobotParameters;

	std::vector<std::shared_ptr<SimulationDataLayer>> m_Simulations;

protected:
	AnimationClip m_AnimationClip;

	float m_SimulationTime;

	bool m_Running;
};
