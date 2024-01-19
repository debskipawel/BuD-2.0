#include "SimulationDataLayer.h"

SimulationDataLayer::SimulationDataLayer()
	: m_ArmLength(5.0f), m_Radius(1.0f), m_AngularVelocity(30.0f), m_Running(false), m_TimeSinceLastUpdate(0.0f), m_SimulationTime(0.0f)
{
}

auto SimulationDataLayer::Run() -> void
{
	if (m_Running)
	{
		return;
	}

	m_TimeSinceLastUpdate = 0.0f;
	m_SimulationTime = 0.0f;
	m_Running = true;
}

auto SimulationDataLayer::Stop() -> void
{
	if (!m_Running)
	{
		return;
	}

	m_TimeSinceLastUpdate = 0.0f;
	m_SimulationTime = 0.0f;
	m_Running = false;
}

auto SimulationDataLayer::IsRunning() const -> bool
{
	return m_Running;
}

void SimulationDataLayer::Update(float deltaTime)
{
	if (!m_Running)
	{
		return;
	}

	constexpr auto TICK_PERIOD = 1.0f / 120.0f;

	m_TimeSinceLastUpdate += deltaTime;

	while (m_TimeSinceLastUpdate > TICK_PERIOD)
	{
		m_SimulationTime += TICK_PERIOD;
		m_TimeSinceLastUpdate -= TICK_PERIOD;

		//Update(m_SimulationTime);
	}
}
