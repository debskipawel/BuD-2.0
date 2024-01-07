#include "FrameInterpolationSimulation.h"

#include <Robot/Calculators/EffectorFrameCalculator.h>
#include <Robot/InverseKinematicSolver.h>

FrameInterpolationSimulation::FrameInterpolationSimulation()
{
}

auto FrameInterpolationSimulation::Interpolate(const RobotParameters& parameters, const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2, float t) -> RobotConfiguration
{
	auto configuration1 = frame1.GetConfiguration();
	auto configuration2 = frame2.GetConfiguration();

	auto effectorFrameCalculator = EffectorFrameCalculator();

	auto effectorFrame1 = effectorFrameCalculator.Calculate(configuration1, parameters);
	auto effectorFrame2 = effectorFrameCalculator.Calculate(configuration2, parameters);

	auto interpolatedFrame = InterpolateFrame(effectorFrame1, effectorFrame2, t);
	
	m_Scene.UpdateFrameMesh(interpolatedFrame);

	auto inverseKinematicSolver = InverseKinematicSolver();

	auto resultConfiguration = inverseKinematicSolver.Solve(parameters, Frame(), interpolatedFrame, t < 0.5f ? configuration1 : configuration2);

	return resultConfiguration;
}

auto FrameInterpolationSimulation::InterpolateFrame(const Frame& frame1, const Frame& frame2, float t) -> Frame
{
	auto effectorPosition1 = frame1.Position();
	auto effectorPosition2 = frame2.Position();

	auto effectorRotation1 = dxm::Quaternion::CreateFromRotationMatrix(
		dxm::Matrix::CreateLookAt(dxm::Vector3::Zero, frame1.Front(), frame1.Up())
	);

	auto effectorRotation2 = dxm::Quaternion::CreateFromRotationMatrix(
		dxm::Matrix::CreateLookAt(dxm::Vector3::Zero, frame2.Front(), frame2.Up())
	);

	auto interpolatedEffectorPosition = dxm::Vector3::Lerp(effectorPosition1, effectorPosition2, t);
	auto interpolatedEffectorRotation = dxm::Quaternion::Slerp(effectorRotation1, effectorRotation2, t);
	interpolatedEffectorRotation.w *= -1;

	auto resultFrame = Frame();

	resultFrame = resultFrame
		.Translate(interpolatedEffectorPosition)
		.Rotate(interpolatedEffectorRotation);

	return resultFrame;
}
