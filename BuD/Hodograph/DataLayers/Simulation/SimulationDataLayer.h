#pragma once

#include <BuD.h>

struct SimulationDataLayer
{
public:
	SimulationDataLayer();

	virtual auto Run() -> void;
	virtual auto Stop() -> void;

	virtual auto IsRunning() const -> bool;

	virtual void Update(float deltaTime);

	float m_ArmLength;
	float m_Radius;
	float m_AngularVelocity;

protected:
	bool m_Running;

	float m_SimulationTime;
	float m_TimeSinceLastUpdate;
};
