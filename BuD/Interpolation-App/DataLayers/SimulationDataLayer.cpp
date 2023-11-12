#include "SimulationDataLayer.h"

SimulationDataLayer::SimulationDataLayer()
	: m_Looped(false), m_Duration(5.0f), m_Running(false), m_Time(0.0f)
{
	// TODO: remove- just for benchmark
	m_KeyFrames = { KeyFrame(0.0f), KeyFrame(1.0f), KeyFrame(3.0f), KeyFrame(4.0f), KeyFrame(5.0f) };
}

void SimulationDataLayer::Run()
{
	m_Running = true;
}

void SimulationDataLayer::Stop()
{
	m_Running = false;
}

void SimulationDataLayer::Update(float deltaTime)
{
	if (!m_Running)
	{
		return;
	}

	m_Time += deltaTime;

	if (m_Looped)
	{
		m_Time -= int(m_Time / m_Duration) * m_Duration;
	}
	else if (m_Time >= m_Duration)
	{
		m_Time = m_Duration;
		m_Running = false;
	}
}
