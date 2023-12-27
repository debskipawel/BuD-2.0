#include "InverseKinematicSolver.h"

auto InverseKinematicSolver::Solve(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame) -> RobotConfiguration
{
    auto P0 = baseFrame.Position();
    auto P5 = effectorFrame.Position();

    auto P1 = P0;
    auto P2 = P1 + robotParameters.m_L1 * baseFrame.Up();
    auto P4 = P5 - robotParameters.m_L4 * effectorFrame.Front();

    // E1: <P3 - P4, effectorFrame.Front()> == 0
    // E2: 

    return RobotConfiguration();
}
