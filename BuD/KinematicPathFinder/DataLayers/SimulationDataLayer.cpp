#include "SimulationDataLayer.h"

SimulationDataLayer::SimulationDataLayer()
	: m_CurrentIndex(0), m_Path(), m_CurrentConfiguration({ 0.0f, 0.0f }), m_AngularVelocity(20.0f)
{

}

auto SimulationDataLayer::StartSimulation(std::vector<std::pair<int, int>> path) -> void
{
	m_Path = path;

	if (m_Path.empty())
	{
		return;
	}

	m_CurrentIndex = 0;
	m_CurrentConfiguration = m_Path.front();
}

auto SimulationDataLayer::Update(float deltaTime) -> void
{
	auto step = deltaTime * m_AngularVelocity;

	while (step > 0.0f && m_CurrentIndex + 1 < m_Path.size())
	{
		auto& current = m_Path[m_CurrentIndex];
		auto& next = m_Path[m_CurrentIndex + 1];

		auto currentProgress = std::make_pair(m_CurrentConfiguration.first - current.first, m_CurrentConfiguration.second - current.second);
		auto difference = std::make_pair(next.first - current.first, next.second - current.second);

		auto t = max(fabsf(currentProgress.first), fabsf(currentProgress.second)) / max(fabsf(difference.first), fabsf(difference.second));

		auto move = min(step, 1.0f - t);

		m_CurrentConfiguration = { m_CurrentConfiguration.first + move * difference.first, m_CurrentConfiguration.second + move * difference.second };

		step -= move;

		if (step > 0.0f)
		{
			m_CurrentIndex++;
		}
	}
}

auto SimulationDataLayer::IsFinished() -> bool
{
	return m_CurrentIndex + 1 >= m_Path.size();
}

auto SimulationDataLayer::GetConfiguration() -> std::pair<float, float>
{
	return m_CurrentConfiguration;
}
