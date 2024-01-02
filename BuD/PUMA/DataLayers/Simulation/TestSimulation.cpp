#include "TestSimulation.h"

auto TestSimulation::Interpolate(const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration
{
    auto configuration1 = frame1.GetConfiguration();
    auto configuration2 = frame2.GetConfiguration();

    auto resultConfiguration = RobotConfiguration();

    resultConfiguration.m_A1 = (1 - t) * configuration1.m_A1 + t * configuration2.m_A1;
    resultConfiguration.m_A2 = (1 - t) * configuration1.m_A2 + t * configuration2.m_A2;
    resultConfiguration.m_A3 = (1 - t) * configuration1.m_A3 + t * configuration2.m_A3;
    resultConfiguration.m_A4 = (1 - t) * configuration1.m_A4 + t * configuration2.m_A4;
    resultConfiguration.m_A5 = (1 - t) * configuration1.m_A5 + t * configuration2.m_A5;
    resultConfiguration.m_Q2 = (1 - t) * configuration1.m_Q2 + t * configuration2.m_Q2;

    return resultConfiguration;
}
