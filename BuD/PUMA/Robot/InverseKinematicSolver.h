#pragma once

#include <BuD.h>

#include <Robot/Frame.h>
#include <Robot/RobotConfiguration.h>
#include <Robot/RobotParameters.h>

class InverseKinematicSolver
{
public:
	virtual auto Solve(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration = std::nullopt) -> RobotConfiguration;

protected:
	virtual auto SolveAdjacentPoints(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration = std::nullopt) -> RobotConfiguration;
	virtual auto SolveColinear(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration = std::nullopt) -> RobotConfiguration;
	virtual auto SolveWithPlane(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration = std::nullopt) -> RobotConfiguration;
};
