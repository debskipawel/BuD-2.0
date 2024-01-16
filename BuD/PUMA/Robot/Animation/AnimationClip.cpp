#include "AnimationClip.h"

#include <algorithm>

AnimationClip::AnimationClip(float duration, const std::vector<AnimationKeyFrame>& keyFrames)
	: m_Duration(duration), m_KeyFrames(keyFrames)
{
}

auto AnimationClip::Duration() const -> float
{
	return m_Duration;
}

auto AnimationClip::AddKeyFrame(const AnimationKeyFrame& keyFrame) -> void
{
	m_KeyFrames.push_back(keyFrame);

	SortFrames();
}

auto AnimationClip::RemoveKeyFrame(unsigned int frameId) -> void
{
	std::erase_if(m_KeyFrames, [frameId](const AnimationKeyFrame& keyFrame) { return keyFrame.Id() == frameId; });
}

auto AnimationClip::KeyFrames() -> std::vector<AnimationKeyFrame>&
{
	return m_KeyFrames;
}

auto AnimationClip::KeyFrames() const -> const std::vector<AnimationKeyFrame>&
{
	return m_KeyFrames;
}

auto AnimationClip::SortFrames() -> void
{
	std::sort(m_KeyFrames.begin(), m_KeyFrames.end(),
		[](const AnimationKeyFrame& frame1, const AnimationKeyFrame& frame2)
		{
			return frame1.GetTime() < frame2.GetTime();
		}
	);
}
