#pragma once

#include <BuD.h>

#include <Robot/Frame.h>
#include <Robot/RobotConfiguration.h>
#include <Robot/RobotParameters.h>

class InverseKinematicSolver
{
public:
	virtual auto Solve(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame) -> RobotConfiguration;
};
