#include "AnimationClip.h"

AnimationClip::AnimationClip(const std::vector<KeyFrame>& keyFrames, float duration)
	: m_KeyFrames(keyFrames), m_Duration(duration), m_SlerpQuaternionInterpolation(false)
{
}

void AnimationClip::AddKeyFrame(float time)
{
	auto keyFrame = Interpolate(time);

	AddKeyFrame(keyFrame);
}

void AnimationClip::AddKeyFrame(const KeyFrame& keyFrame)
{
	m_KeyFrames.push_back(keyFrame);

	SortKeyFrames();
}

bool AnimationClip::RemoveKeyFrame(unsigned int id)
{
	auto erased = std::erase_if(m_KeyFrames, [id](const KeyFrame& keyFrame) { return keyFrame.Id() == id; });

	return erased > 0;
}

bool AnimationClip::RemoveKeyFrame(const KeyFrame& keyFrame)
{
	return RemoveKeyFrame(keyFrame.Id());
}

const std::vector<KeyFrame>& AnimationClip::GetKeyFrames() const
{
	return m_KeyFrames;
}

KeyFrame& AnimationClip::GetKeyFrame(unsigned int id)
{
	auto selectedKeyFrame = std::find_if(m_KeyFrames.begin(), m_KeyFrames.end(), [id](const KeyFrame& keyFrame) { return keyFrame.Id() == id; });

	return *selectedKeyFrame;
}

void AnimationClip::SortKeyFrames()
{
	std::sort(m_KeyFrames.begin(), m_KeyFrames.end(),
		[](const KeyFrame& frame1, const KeyFrame& frame2)
		{
			return frame1.m_TimePoint < frame2.m_TimePoint;
		}
	);
}

KeyFrame AnimationClip::Interpolate(float time)
{
	if (m_KeyFrames.empty())
	{
		return KeyFrame(time);
	}

	auto next = std::upper_bound(m_KeyFrames.begin(), m_KeyFrames.end(), time,
		[](float value, const KeyFrame& keyframe)
		{
			return value < keyframe.m_TimePoint;
		}
	);

	if (next == m_KeyFrames.begin())
	{
		return KeyFrame(time, next->m_Position, next->m_EulerAngles, next->m_Quaternion);
	}

	auto prev = next - 1;

	if (next == m_KeyFrames.end())
	{
		return KeyFrame(time, prev->m_Position, prev->m_EulerAngles, prev->m_Quaternion);
	}

	// ACTUAL INTERPOLATION LOGIC
	// TODO: now Euler angles are interpolated naively, just for sake of benchmark
	auto t = (time - prev->m_TimePoint) / (next->m_TimePoint - prev->m_TimePoint);

	auto position = dxm::Vector3::Lerp(prev->m_Position, next->m_Position, t);
	auto eulerAngles = dxm::Vector3::Lerp(prev->m_EulerAngles, next->m_EulerAngles, t);
	auto quaternion = m_SlerpQuaternionInterpolation
		? dxm::Quaternion::Slerp(prev->m_Quaternion, next->m_Quaternion, t)
		: dxm::Quaternion::Lerp(prev->m_Quaternion, next->m_Quaternion, t);

	return KeyFrame(time, position, eulerAngles, quaternion);
}

constexpr float AnimationClip::GetDuration() const
{
	return m_Duration;
}

void AnimationClip::SetDuration(float duration)
{
	if (duration >= 0.0f)
	{
		m_Duration = duration;
	}
}
