#include "TestSimulation.h"

auto TestSimulation::Interpolate(const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration
{
    return frame1.Configuration();
}
