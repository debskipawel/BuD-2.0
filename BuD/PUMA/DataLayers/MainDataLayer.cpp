#include "MainDataLayer.h"

#include <DataLayers/Simulation/TestSimulation.h>

MainDataLayer::MainDataLayer()
	: m_Simulations(), m_Running(false), m_Looped(false), m_SimulationTime(0.0f), m_RobotParameters(2.0f, 1.0f, 1.0f), m_AnimationClip(5.0f)
{
	m_Simulations.emplace_back(std::make_shared<TestSimulation>());
	m_Simulations.emplace_back(std::make_shared<TestSimulation>());
}

auto MainDataLayer::Update(float deltaTime) -> void
{
	if (!IsRunning())
	{
		return;
	}

	SetSimulationTime(m_SimulationTime + deltaTime);
}

auto MainDataLayer::Start() -> void
{
	if (IsRunning())
	{
		return;
	}

	if (m_SimulationTime >= m_AnimationClip.Duration())
	{
		m_SimulationTime = 0.0f;
	}

	m_Running = true;
}

auto MainDataLayer::Stop() -> void
{
	if (!IsRunning())
	{
		return;
	}

	m_Running = false;
}

auto MainDataLayer::ToggleLoop() -> void
{
	m_Looped = !m_Looped;
}

auto MainDataLayer::IsRunning() const -> bool
{
	return m_Running;
}

auto MainDataLayer::IsLooped() const -> bool
{
	return m_Looped;
}

auto MainDataLayer::GetSimulationTime() const -> float
{
	return m_SimulationTime;
}

auto MainDataLayer::SetSimulationTime(float simulationTime) -> void
{
	m_SimulationTime = simulationTime;

	auto duration = m_AnimationClip.Duration();

	if (m_Looped)
	{
		m_SimulationTime -= static_cast<int>(floorf(m_SimulationTime / duration)) * duration;
	}

	if (m_SimulationTime > duration)
	{
		m_SimulationTime = duration;
		Stop();
	}

	for (const auto& simulation : m_Simulations)
	{
		simulation->Update(m_RobotParameters, m_AnimationClip, m_SimulationTime);
	}
}
