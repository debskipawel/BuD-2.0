#include "ParameterInterpolationSimulation.h"

ParameterInterpolationSimulation::ParameterInterpolationSimulation()
{
}

auto ParameterInterpolationSimulation::Interpolate(const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration
{
	auto configuration1 = frame1.GetConfiguration();
	auto configuration2 = frame2.GetConfiguration();

	configuration1.m_A1 -= static_cast<int>(floorf(configuration1.m_A1 / 360.0f)) * 360.0f;
	configuration1.m_A2 -= static_cast<int>(floorf(configuration1.m_A2 / 360.0f)) * 360.0f;
	configuration1.m_A3 -= static_cast<int>(floorf(configuration1.m_A3 / 360.0f)) * 360.0f;
	configuration1.m_A4 -= static_cast<int>(floorf(configuration1.m_A4 / 360.0f)) * 360.0f;
	configuration1.m_A5 -= static_cast<int>(floorf(configuration1.m_A5 / 360.0f)) * 360.0f;

	configuration2.m_A1 -= static_cast<int>(floorf(configuration2.m_A1 / 360.0f)) * 360.0f;
	configuration2.m_A2 -= static_cast<int>(floorf(configuration2.m_A2 / 360.0f)) * 360.0f;
	configuration2.m_A3 -= static_cast<int>(floorf(configuration2.m_A3 / 360.0f)) * 360.0f;
	configuration2.m_A4 -= static_cast<int>(floorf(configuration2.m_A4 / 360.0f)) * 360.0f;
	configuration2.m_A5 -= static_cast<int>(floorf(configuration2.m_A5 / 360.0f)) * 360.0f;

	auto interpolatedConfiguration = RobotConfiguration();

	auto dA1 = configuration2.m_A1 - configuration1.m_A1;
	auto dA2 = configuration2.m_A2 - configuration1.m_A2;
	auto dA3 = configuration2.m_A3 - configuration1.m_A3;
	auto dA4 = configuration2.m_A4 - configuration1.m_A4;
	auto dA5 = configuration2.m_A5 - configuration1.m_A5;

	if (fabsf(dA1) > 180.0f)
	{
		dA1 = (dA1 > 0.0f) ? (dA1 - 360.0f) : (dA1 + 360.0f);
	}

	if (fabsf(dA2) > 180.0f)
	{
		dA2 = (dA2 > 0.0f) ? (dA2 - 360.0f) : (dA2 + 360.0f);
	}

	if (fabsf(dA3) > 180.0f)
	{
		dA3 = (dA3 > 0.0f) ? (dA3 - 360.0f) : (dA3 + 360.0f);
	}

	if (fabsf(dA4) > 180.0f)
	{
		dA4 = (dA4 > 0.0f) ? (dA4 - 360.0f) : (dA4 + 360.0f);
	}

	if (fabsf(dA5) > 180.0f)
	{
		dA5 = (dA5 > 0.0f) ? (dA5 - 360.0f) : (dA5 + 360.0f);
	}

	interpolatedConfiguration.m_A1 = configuration1.m_A1 + t * dA1;
	interpolatedConfiguration.m_A2 = configuration1.m_A2 + t * dA2;
	interpolatedConfiguration.m_A3 = configuration1.m_A3 + t * dA3;
	interpolatedConfiguration.m_A4 = configuration1.m_A4 + t * dA4;
	interpolatedConfiguration.m_A5 = configuration1.m_A5 + t * dA5;

	interpolatedConfiguration.m_A1 -= static_cast<int>(floorf(interpolatedConfiguration.m_A1 / 360.0f)) * 360.0f;
	interpolatedConfiguration.m_A2 -= static_cast<int>(floorf(interpolatedConfiguration.m_A2 / 360.0f)) * 360.0f;
	interpolatedConfiguration.m_A3 -= static_cast<int>(floorf(interpolatedConfiguration.m_A3 / 360.0f)) * 360.0f;
	interpolatedConfiguration.m_A4 -= static_cast<int>(floorf(interpolatedConfiguration.m_A4 / 360.0f)) * 360.0f;
	interpolatedConfiguration.m_A5 -= static_cast<int>(floorf(interpolatedConfiguration.m_A5 / 360.0f)) * 360.0f;

	interpolatedConfiguration.m_Q2 = (1 - t) * configuration1.m_Q2 + t * configuration2.m_Q2;

	return interpolatedConfiguration;
}
