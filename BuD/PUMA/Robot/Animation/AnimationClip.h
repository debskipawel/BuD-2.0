#pragma once

#include <vector>

#include <Robot/Animation/AnimationKeyFrame.h>

class AnimationClip
{
public:
	AnimationClip(float duration, const std::vector<AnimationKeyFrame>& keyFrames = {});

	virtual auto Duration() const -> float;

	virtual auto AddKeyFrame(const AnimationKeyFrame& keyFrame) -> void;
	virtual auto RemoveKeyFrame(unsigned int frameId) -> void;

	virtual auto KeyFrames() -> std::vector<AnimationKeyFrame>&;
	virtual auto KeyFrames() const -> const std::vector<AnimationKeyFrame>&;

	virtual auto SortFrames() -> void;

protected:
	float m_Duration;

	std::vector<AnimationKeyFrame> m_KeyFrames;
};
