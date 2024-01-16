#pragma once

#include <Robot/RobotConfiguration.h>
#include <Robot/RobotParameters.h>
#include <Robot/Frame.h>

class AllJointFramesCalculator
{
public:
	virtual auto Calculate(const RobotConfiguration& configuration, const RobotParameters& parameters) -> std::array<Frame, 6>;
};
