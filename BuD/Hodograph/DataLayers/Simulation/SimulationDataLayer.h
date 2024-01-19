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
	float m_StandardDeviation;

	std::vector<float> m_TimeValues;
	std::vector<float> m_ArmLengthValues;
	std::vector<float> m_PositionValues;
	std::vector<float> m_VelocityValues;
	std::vector<float> m_AccelerationValues;

protected:
	virtual auto ClearPlotData() -> void;
	
	virtual auto SimulationTick() -> void;

	bool m_Running;

	float m_Angle;
	float m_SimulationTime;
	float m_TimeSinceLastUpdate;
};
