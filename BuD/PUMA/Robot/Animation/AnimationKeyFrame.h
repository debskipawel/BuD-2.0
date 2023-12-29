#pragma once

#include <string>

#include <Robot/RobotConfiguration.h>

class AnimationKeyFrame
{
public:
	AnimationKeyFrame(float frameTime, const RobotConfiguration& configuration);

	virtual auto GetTime() const -> float;
	virtual auto SetTime(float time) -> void;

	virtual auto Configuration() const -> RobotConfiguration;
	virtual auto Id() const -> int;

	std::string m_Name;

protected:
	float m_FrameTime;
	RobotConfiguration m_RobotConfiguration;

	int m_Id;
};
