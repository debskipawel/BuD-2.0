#include "SimulationDataLayer.h"

#include <numbers>
#include <random>

constexpr auto TICK_PERIOD = 1.0f / 120.0f;

SimulationDataLayer::SimulationDataLayer()
	: m_ArmLength(5.0f), m_Radius(1.0f), m_AngularVelocity(30.0f), m_Running(false), m_TimeSinceLastUpdate(0.0f), m_SimulationTime(0.0f), m_StandardDeviation(0.0001f)
{
}

auto SimulationDataLayer::Run() -> void
{
	if (m_Running)
	{
		return;
	}

	ClearPlotData();

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

	m_TimeSinceLastUpdate += deltaTime;

	while (m_TimeSinceLastUpdate > TICK_PERIOD)
	{
		m_SimulationTime += TICK_PERIOD;
		m_TimeSinceLastUpdate -= TICK_PERIOD;

		SimulationTick();
	}
}

auto SimulationDataLayer::ClearPlotData() -> void
{
	m_TimeValues.clear();
	m_ArmLengthValues.clear();
	m_PositionValues.clear();
	m_VelocityValues.clear();
	m_AccelerationValues.clear();
}

auto SimulationDataLayer::SimulationTick() -> void
{
	auto positionCalculator = [this](float time)
	{
		auto omega = m_AngularVelocity * std::numbers::pi_v<float> / 180.0f;

		static auto generator = std::default_random_engine();
		auto distribution = std::normal_distribution<float>(0.0f, m_StandardDeviation);

		auto disturbance = distribution(generator);

		auto R = m_Radius;
		auto L = (1.0f + disturbance) * m_ArmLength;

		auto alpha = omega * time;

		auto ca = cosf(alpha);

		return R * ca + sqrtf(R * R * ca * ca - R * R + L * L) - sqrtf(L * L - R * R);
	};

	auto positionCurrent = positionCalculator(m_SimulationTime);
	auto positionPrev = positionCalculator(m_SimulationTime - TICK_PERIOD);
	auto positionNext = positionCalculator(m_SimulationTime + TICK_PERIOD);

	auto velocityCurrent = (positionNext - positionCurrent) / TICK_PERIOD;
	auto velocityPrev = (positionCurrent - positionPrev) / TICK_PERIOD;

	auto accelerationCurrent = (positionNext - 2.0f * positionCurrent + positionPrev) / (TICK_PERIOD * TICK_PERIOD);

	m_TimeValues.push_back(m_SimulationTime);
	
	m_PositionValues.push_back(positionCurrent);
	m_VelocityValues.push_back(velocityCurrent);
	m_AccelerationValues.push_back(accelerationCurrent);
}
