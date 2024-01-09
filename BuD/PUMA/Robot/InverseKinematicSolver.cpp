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

	//if (v21.LengthSquared() < 1e-10f || v41.LengthSquared() < 1e-10f)
	//{
	//	// some points are common
	//	return SolveAdjacentPoints(robotParameters, baseFrame, effectorFrame, referenceConfiguration);
	//}

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
	auto P0 = baseFrame.Position();
	auto P5 = effectorFrame.Position();

	auto P1 = P0;
	auto P2 = P1 + robotParameters.m_L1 * baseFrame.Up();
	auto P4 = P5 - robotParameters.m_L4 * effectorFrame.Right();

	auto allJointsCalculator = AllJointFramesCalculator();

	auto P3 = dxm::Vector3::Zero;

	if (referenceConfiguration.has_value())
	{
		auto [F0prev, F1prev, F2prev, F3prev, F4prev, F5prev] = allJointsCalculator.Calculate(referenceConfiguration.value(), robotParameters);

		auto sphereRadiusVector = F3prev.Position() - P4;

		if (sphereRadiusVector.Length() < 1e-5f)
		{
			sphereRadiusVector = F3prev.Position() - F4prev.Position();
		}

		sphereRadiusVector.Normalize();

		P3 = P4 + robotParameters.m_L3 * sphereRadiusVector;
	}
	else
	{
		auto v42 = P2 - P4;
		auto distanceHalved = 0.5f * v42.Length();

		if (distanceHalved < robotParameters.m_L3)
		{
			P3 = P4 + distanceHalved * v42 + sqrtf(robotParameters.m_L3 * robotParameters.m_L3 - distanceHalved * distanceHalved) * dxm::Vector3::UnitY;
		}
		else
		{
			v42.Normalize();

			P3 = P4 + robotParameters.m_L3 * v42;
		}
	}

	auto resultConfiguration = GetAnglesFromJoints(robotParameters, baseFrame, effectorFrame, { P1, P2, P3, P4, P5 }, referenceConfiguration);

	return resultConfiguration;
}

auto InverseKinematicSolver::SolveWithPlane(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, const std::optional<RobotConfiguration> referenceConfiguration) -> RobotConfiguration
{
	auto P0 = baseFrame.Position();
	auto P5 = effectorFrame.Position();

	auto X5 = effectorFrame.Right();
	auto L3 = robotParameters.m_L3;

	auto P1 = P0;
	auto P2 = P1 + robotParameters.m_L1 * baseFrame.Up();
	auto P4 = P5 - robotParameters.m_L4 * effectorFrame.Right();

	auto v21 = P2 - P1;
	auto v41 = P4 - P1;

	v21.Normalize();
	v41.Normalize();

	auto N = v21.Cross(v41);
	N.Normalize();

	auto crossXY = N.x * X5.y - N.y * X5.x;
	auto crossXZ = N.z * X5.x - N.x * X5.z;
	auto crossYZ = N.y * X5.z - N.z * X5.y;

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

	auto resultConfiguration = GetAnglesFromJoints(robotParameters, baseFrame, effectorFrame, { P1, P2, P3, P4, P5 }, referenceConfiguration);

	return resultConfiguration;
}

auto InverseKinematicSolver::GetAnglesFromJoints(const RobotParameters& robotParameters, const Frame& baseFrame, const Frame& effectorFrame, std::array<dxm::Vector3, 5> joints, const std::optional<RobotConfiguration> referenceConfiguration) -> RobotConfiguration
{
	auto [P1, P2, P3, P4, P5] = joints;

	// ----- ALPHA 1 CALCULATION -----
	auto v41 = P4 - P1;
	auto v31 = P3 - P1;

	v41.Normalize();
	v31.Normalize();

	auto d0 = powf(v41.Dot(baseFrame.Right()), 2.0f) + powf(v41.Dot(baseFrame.Front()), 2.0f);
	auto d1 = powf(v31.Dot(baseFrame.Right()), 2.0f) + powf(v31.Dot(baseFrame.Front()), 2.0f);

	auto alpha1 = 0.0f;
	auto canBeTrustedAlpha1 = true;

	if (d0 > 1e-5f || d1 > 1e-5f)
	{
		auto a1 = (d0 > d1) ? v41 : v31;
		
		alpha1 = atan2f(a1.Dot(baseFrame.Front()), a1.Dot(baseFrame.Right()));
	}
	else
	{
		canBeTrustedAlpha1 = false;

		if (referenceConfiguration.has_value())
		{
			alpha1 = referenceConfiguration->m_A1;
		}
	}

	auto F1 = baseFrame.Rotate(dxm::Matrix::CreateFromAxisAngle(baseFrame.Up(), alpha1));

	// ----- ALPHA 2 CALCULATION -----
	auto alpha2 = 0.0f;
	auto canBeTrustedAlpha2 = true;
	
	auto x2 = P3 - P2;
	x2.Normalize();

	if (x2.Length() > 1e-5f)
	{
		alpha2 = -atan2f(x2.Dot(F1.Up()), x2.Dot(F1.Right()));
	}
	else
	{
		canBeTrustedAlpha2 = false;

		if (referenceConfiguration.has_value())
		{
			alpha2 = referenceConfiguration->m_A2;
		}
	}

	auto F2 = F1.Translate(P2 - P1).Rotate(dxm::Matrix::CreateFromAxisAngle(F1.Front(), alpha2));

	// ----- ALPHA 3 CALCULATION -----
	auto alpha3 = 0.0f;
	auto canBeTrustedAlpha3 = true;

	auto y3 = P3 - P4;

	if (y3.Length() > 1e-5f)
	{
		y3.Normalize();

		auto x2 = F2.Right();
		auto y2 = F2.Up();
		auto z2 = F2.Front();
		auto x3 = z2.Cross(y3);

		alpha3 = -atan2f(x3.Dot(y2), x3.Dot(x2));
	}
	else
	{
		canBeTrustedAlpha3 = false;

		if (referenceConfiguration.has_value())
		{
			alpha3 = referenceConfiguration->m_A3;
		}
	}

	auto F3 = F2.Translate(P3 - P2).Rotate(dxm::Matrix::CreateFromAxisAngle(F2.Front(), alpha3));

	// ----- ALPHA 4 CALCULATION -----
	auto x5 = effectorFrame.Right();
	auto x3 = F3.Right();
	auto z3 = F3.Front();

	auto alpha4 = atan2f(x5.Dot(z3), x5.Dot(x3));

	// ----- ALPHA 5 CALCULATION -----
	auto alpha5 = 0.0f;
	auto canBeTrustedAlpha5 = true;

	auto y4 = y3;

	if (!canBeTrustedAlpha3)
	{
		if (referenceConfiguration.has_value())
		{
			auto allFramesCalculator = AllJointFramesCalculator();

			auto [pF0, pF1, pF2, pF3, pF4, pF5] = allFramesCalculator.Calculate(referenceConfiguration.value(), robotParameters);

			y4 = pF3.Up();
		}
		else
		{
			y4 = dxm::Vector3::UnitY;
		}
	}

	auto z4 = y4.Cross(x5);
	auto y5 = effectorFrame.Up();

	alpha5 = std::numbers::pi_v<float> / 2.0f - atan2f(y5.Dot(y4), -y5.Dot(z4));

	// ----- FINAL ASSEMBLY -----
	auto resultConfiguration = RobotConfiguration();
	resultConfiguration.m_Q2 = (P3 - P2).Length();
	resultConfiguration.m_A1 = DirectX::XMConvertToDegrees(alpha1);
	resultConfiguration.m_A2 = DirectX::XMConvertToDegrees(alpha2);
	resultConfiguration.m_A3 = DirectX::XMConvertToDegrees(alpha3);
	resultConfiguration.m_A4 = DirectX::XMConvertToDegrees(alpha4);
	resultConfiguration.m_A5 = DirectX::XMConvertToDegrees(alpha5);

	auto allFramesCalculator = AllJointFramesCalculator();

	auto [cF0, cF1, cF2, cF3, cF4, cF5] = allFramesCalculator.Calculate(resultConfiguration, robotParameters);

	auto dP1 = cF1.Position() - P1;
	auto dP2 = cF2.Position() - P2;
	auto dP3 = cF3.Position() - P3;
	auto dP4 = cF4.Position() - P4;
	auto dP5 = cF5.Position() - P5;

	// debugging purposes
	auto totalDifference = dP1.Length() + dP2.Length() + dP3.Length() + dP4.Length() + dP5.Length();

	if (totalDifference > 0.1f)
	{
		int a = 5;
	}

	return resultConfiguration;
}
