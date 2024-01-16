#include "AnimationKeyFrame.h"

#include <random>

AnimationKeyFrame::AnimationKeyFrame(float frameTime, const RobotConfiguration& configuration)
	: m_FrameTime(frameTime), m_RobotConfiguration(configuration)
{
	std::random_device rd;
	m_Id = rd();
}

auto AnimationKeyFrame::GetTime() const -> float
{
	return m_FrameTime;
}

auto AnimationKeyFrame::SetTime(float time) -> void
{
	m_FrameTime = time;
}

auto AnimationKeyFrame::GetConfiguration() const -> RobotConfiguration
{
	return m_RobotConfiguration;
}

auto AnimationKeyFrame::SetConfiguration(const RobotConfiguration& configuration) -> void
{
	m_RobotConfiguration = configuration;
}

auto AnimationKeyFrame::Id() const -> unsigned int
{
	return m_Id;
}
