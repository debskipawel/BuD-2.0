#pragma once

#include <BuD.h>

#include <KeyFrame.h>

struct SimulationDataLayer
{
	SimulationDataLayer();

	virtual void Run();
	virtual void Stop();

	virtual void Update(float deltaTime);
	virtual KeyFrame Interpolate();

	bool m_Running;
	bool m_Looped;

	float m_Duration;
	float m_Time;

	std::vector<KeyFrame> m_KeyFrames;
};
