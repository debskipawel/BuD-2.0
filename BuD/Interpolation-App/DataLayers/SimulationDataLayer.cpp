#include "SimulationDataLayer.h"

BaseSimulation::BaseSimulation()
	: m_Looped(false), m_Running(false), m_Time(0.0f), m_AnimationClip({}, 5.0f)
{
}

void BaseSimulation::Run()
{
	if (m_Time == m_AnimationClip.GetDuration())
	{
		m_Time = 0.0f;
	}

	m_Running = true;
}

void BaseSimulation::Stop()
{
	m_Running = false;
}

void BaseSimulation::Update(float deltaTime)
{
	if (!m_Running)
	{
		return;
	}

	m_Time += deltaTime;

	auto duration = m_AnimationClip.GetDuration();

	if (m_Looped)
	{
		m_Time -= int(m_Time / duration) * duration;
	}
	else if (m_Time >= duration)
	{
		m_Time = duration;
		m_Running = false;
	}
}

KeyFrame BaseSimulation::Interpolate()
{
	return m_AnimationClip.Interpolate(m_Time);
}
