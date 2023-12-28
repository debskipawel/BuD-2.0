#pragma once

#include <Robot/RobotConfiguration.h>

class AnimationKeyFrame
{
public:
	AnimationKeyFrame(float frameTime, const RobotConfiguration& configuration);

	virtual auto Time() const -> float;
	virtual auto Configuration() const -> RobotConfiguration;

protected:
	float m_FrameTime;
	RobotConfiguration m_RobotConfiguration;
};
