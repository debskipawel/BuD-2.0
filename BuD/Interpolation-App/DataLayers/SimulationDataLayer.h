#pragma once

#include <BuD.h>

#include <AnimationClip.h>

struct SimulationDataLayer
{
	SimulationDataLayer();

	virtual void Run();
	virtual void Stop();

	virtual void Update(float deltaTime);
	virtual KeyFrame Interpolate();

	bool m_Running;
	bool m_Looped;

	float m_Time;

	AnimationClip m_AnimationClip;
};
