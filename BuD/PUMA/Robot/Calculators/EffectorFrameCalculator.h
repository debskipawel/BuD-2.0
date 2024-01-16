#pragma once

#include <Robot/RobotConfiguration.h>
#include <Robot/RobotParameters.h>
#include <Robot/Frame.h>

class EffectorFrameCalculator
{
public:
	virtual auto Calculate(const RobotConfiguration& configuration, const RobotParameters& parameters) -> Frame;
};
