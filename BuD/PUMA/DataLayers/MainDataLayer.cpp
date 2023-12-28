#include "MainDataLayer.h"

MainDataLayer::MainDataLayer()
	: m_Simulations(), m_Running(false)
{
	m_Simulations.emplace_back(std::make_shared<SimulationDataLayer>());
	m_Simulations.emplace_back(std::make_shared<SimulationDataLayer>());
}

auto MainDataLayer::Update(float deltaTime) -> void
{
	if (!m_Running)
	{
		return;
	}

	for (const auto& simulation : m_Simulations)
	{
		simulation->Update(deltaTime);
	}
}

auto MainDataLayer::Start() -> void
{
	if (IsRunning())
	{
		return;
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

auto MainDataLayer::IsRunning() const -> bool
{
	return m_Running;
}
