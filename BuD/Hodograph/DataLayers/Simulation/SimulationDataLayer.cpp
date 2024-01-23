#include "SimulationDataLayer.h"

#include <numbers>
#include <random>

constexpr auto TICK_PERIOD = 1.0f / 120.0f;

SimulationDataLayer::SimulationDataLayer()
	: m_ArmLength(5.0f), m_Radius(1.0f), m_AngularVelocity(60.0f), m_Running(false), m_TimeSinceLastUpdate(0.0f), m_SimulationTime(0.0f), m_StandardDeviation(0.0f), m_Angle(0.0f)
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
	m_Angle = 0.0f;
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
	m_Angle = 0.0f;
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

auto SimulationDataLayer::Alpha() const -> float
{
	return m_Angle;
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
	auto armLengthScalingFactor = 1.0f;
	
	static auto generator = std::default_random_engine();
	
	if (m_StandardDeviation > 0.0f)
	{
		auto distribution = std::normal_distribution<float>(0.0f, m_StandardDeviation);

		auto disturbance = distribution(generator);

		armLengthScalingFactor += disturbance;
	}

	auto omega = m_AngularVelocity * std::numbers::pi_v<float> / 180.0f;

	m_Angle += omega * TICK_PERIOD;

	auto R = m_Radius;
	auto L = armLengthScalingFactor * m_ArmLength;

	auto positionCalculator = [this](float alpha, float L, float R)
	{
		auto ca = cosf(alpha);

		return R * ca + sqrtf(R * R * ca * ca - R * R + L * L) - sqrtf(L * L - R * R);
	};

	auto positionCurrent = positionCalculator(m_Angle, L, R);
	auto positionPrev = positionCalculator(m_Angle - omega * TICK_PERIOD, L, R);
	auto positionPrevPrev = positionCalculator(m_Angle - 2.0f * omega * TICK_PERIOD, L, R);

	if (m_PositionValues.size() > 0)
	{
		positionPrev = m_PositionValues[m_PositionValues.size() - 1];
	}

	if (m_PositionValues.size() > 1)
	{
		positionPrevPrev = m_PositionValues[m_PositionValues.size() - 2];
	}

	auto velocityCurrent = (positionCurrent - positionPrev) / TICK_PERIOD;
	auto velocityPrev = (positionPrev - positionPrevPrev) / TICK_PERIOD;

	auto accelerationCurrent = (velocityCurrent - velocityPrev) / TICK_PERIOD;

	m_TimeValues.push_back(m_SimulationTime);
	
	m_PositionValues.push_back(positionCurrent);
	m_VelocityValues.push_back(velocityCurrent);
	m_AccelerationValues.push_back(accelerationCurrent);
}
