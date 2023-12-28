#include "AnimationKeyFrame.h"

AnimationKeyFrame::AnimationKeyFrame(float frameTime, const RobotConfiguration& configuration)
	: m_FrameTime(frameTime), m_RobotConfiguration(configuration)
{
}

auto AnimationKeyFrame::Time() const -> float
{
	return m_FrameTime;
}

auto AnimationKeyFrame::Configuration() const -> RobotConfiguration
{
	return m_RobotConfiguration;
}
