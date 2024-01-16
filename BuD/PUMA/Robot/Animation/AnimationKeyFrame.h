#pragma once

#include <string>

#include <Robot/RobotConfiguration.h>

class AnimationKeyFrame
{
public:
	AnimationKeyFrame(float frameTime, const RobotConfiguration& configuration);

	virtual auto GetTime() const -> float;
	virtual auto SetTime(float time) -> void;

	virtual auto GetConfiguration() const -> RobotConfiguration;
	virtual auto SetConfiguration(const RobotConfiguration& configuration) -> void;
	virtual auto Id() const -> unsigned int;

	std::string m_Name;

protected:
	float m_FrameTime;
	RobotConfiguration m_RobotConfiguration;

	unsigned int m_Id;
};
