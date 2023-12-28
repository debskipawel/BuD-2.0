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

auto AnimationClip::RemoveKeyFrame(size_t frameIndex) -> void
{
	if (frameIndex > m_KeyFrames.size())
	{
		return;
	}

	m_KeyFrames.erase(m_KeyFrames.begin() + frameIndex);
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
			return frame1.Time() < frame2.Time();
		}
	);
}
