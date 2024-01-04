#include "InverseKinematicSolver.h"

#include <numbers>

#include <Robot/Calculators/AllJointFramesCalculator.h>

auto InverseKinematicSolver::Solve(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration) -> RobotConfiguration
{
    auto P0 = baseFrame.Position();
    auto P5 = effectorFrame.Position();

    auto P1 = P0;
    auto P2 = P1 + robotParameters.m_L1 * baseFrame.Up();
    auto P4 = P5 - robotParameters.m_L4 * effectorFrame.Right();

    auto v21 = P2 - P1;
    auto v41 = P4 - P1;

    if (v21.LengthSquared() < 1e-10f || v41.LengthSquared() < 1e-10f)
    {
        // some points are common
        return SolveAdjacentPoints(robotParameters, baseFrame, effectorFrame, referenceConfiguration);
    }

    v21.Normalize();
    v41.Normalize();

    auto planeNormal = v21.Cross(v41);

    if (planeNormal.LengthSquared() < 1e-10f)
    {
        // colinear, no plane can be assumed
        return SolveColinear(robotParameters, baseFrame, effectorFrame, referenceConfiguration);
    }

    // P1, P2 and P4 create a plane
    return SolveWithPlane(robotParameters, baseFrame, effectorFrame, referenceConfiguration);
}

auto InverseKinematicSolver::SolveAdjacentPoints(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration) -> RobotConfiguration
{
    return RobotConfiguration();
}

auto InverseKinematicSolver::SolveColinear(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration) -> RobotConfiguration
{
    return RobotConfiguration();
}

auto InverseKinematicSolver::SolveWithPlane(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration) -> RobotConfiguration
{
    auto P0 = baseFrame.Position();
    auto P5 = effectorFrame.Position();

    auto X5 = effectorFrame.Right();
    auto L3 = robotParameters.m_L3;

    auto P1 = P0;
    auto P2 = P1 + robotParameters.m_L1 * baseFrame.Up();
    auto P4 = P5 - robotParameters.m_L4 * effectorFrame.Front();

    auto v21 = P2 - P1;
    auto v41 = P4 - P1;

    v21.Normalize();
    v41.Normalize();

    auto N = v21.Cross(v41);
    N.Normalize();

    auto crossXY = N.x * X5.y - N.y * X5.x;
    auto crossXZ = N.z * X5.x - N.x * X5.z;
    auto crossYZ = N.z * X5.y - N.y * X5.z;

    auto v3Denominator = sqrtf(crossXY * crossXY + crossXZ * crossXZ + crossYZ * crossYZ);

    auto v34 = L3 * dxm::Vector3(crossYZ / v3Denominator, crossXZ / v3Denominator, crossXY / v3Denominator);

    auto P31 = P4 + v34;
    auto P32 = P4 - v34;

    auto P3 = P31;

    if (referenceConfiguration.has_value())
    {
        auto allFramesCalculator = AllJointFramesCalculator();

        auto [F0, F1, F2, F3, F4, F5] = allFramesCalculator.Calculate(referenceConfiguration.value(), robotParameters);

        auto distance1 = (P31 - F3.Position()).LengthSquared();
        auto distance2 = (P32 - F3.Position()).LengthSquared();

        P3 = (distance1 < distance2) ? P31 : P32;
    }

    auto resultConfiguration = RobotConfiguration();

    auto v20 = P2 - P0;
    auto v32 = P3 - P2;
    auto v40 = P4 - P0;
    auto v43 = P4 - P3;

    v20.Normalize();
    v32.Normalize();
    v40.Normalize();
    v43.Normalize();

    resultConfiguration.m_Q2 = (P3 - P2).Length();
    resultConfiguration.m_A1 = DirectX::XMConvertToDegrees(-atan2f(v40.z, v40.x));
    resultConfiguration.m_A2 = DirectX::XMConvertToDegrees(acosf(v20.Dot(v32)) - std::numbers::pi_v<float> / 2.0f);
    resultConfiguration.m_A3 = DirectX::XMConvertToDegrees(acosf(v32.Dot(v43)) - std::numbers::pi_v<float> / 2.0f);
    resultConfiguration.m_A4 = DirectX::XMConvertToDegrees(acosf(N.Dot(X5)) + std::numbers::pi_v<float> / 2.0f);
    resultConfiguration.m_A5 = DirectX::XMConvertToDegrees(acosf(-v43.Dot(effectorFrame.Up())));

    return resultConfiguration;
}
