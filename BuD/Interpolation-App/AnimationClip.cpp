#include "AnimationClip.h"

#include <execution>

AnimationClip::AnimationClip(const std::vector<KeyFrame>& keyFrames, float duration)
	: m_KeyFrames(keyFrames), m_Duration(duration), m_SlerpQuaternionInterpolation(false)
{
}

void AnimationClip::AddKeyFrame(float time)
{
	auto keyFrame = Interpolate(time);

	auto eulerRot = keyFrame.m_Quaternion.ToEuler();
	
	keyFrame.m_EulerAngles = dxm::Vector3(
		DirectX::XMConvertToDegrees(eulerRot.x),
		DirectX::XMConvertToDegrees(eulerRot.y),
		DirectX::XMConvertToDegrees(eulerRot.z)
	);

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

std::vector<KeyFrame> AnimationClip::GetIntermediateFrames(unsigned int frameCount)
{
	auto intermediateFrames = std::vector<KeyFrame>(frameCount);

	std::for_each(std::execution::par, intermediateFrames.begin(), intermediateFrames.end(),
		[this, frameCount, &intermediateFrames](KeyFrame& keyFrame)
		{
			auto id = &keyFrame - intermediateFrames.data();

			auto time = id * (frameCount == 1 ? 0.0f : m_Duration / (frameCount - 1));
			
			keyFrame = Interpolate(time);
		}
	);

	return intermediateFrames;
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

	auto eulerAngles = InterpolateEuler(prev->m_EulerAngles, next->m_EulerAngles, t);
	auto quaternion = InsterpolateQuaternion(prev->m_Quaternion, next->m_Quaternion, t);

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

dxm::Vector3 AnimationClip::InterpolateEuler(const dxm::Vector3& prev, const dxm::Vector3& next, float t)
{
	auto prevEuler = dxm::Vector3(prev.x - floorf(prev.x / 360.0f) * 360.0f, prev.y - floorf(prev.y / 360.0f) * 360.0f, prev.z - floorf(prev.z / 360.0f) * 360.0f);
	auto nextEuler = dxm::Vector3(next.x - floorf(next.x / 360.0f) * 360.0f, next.y - floorf(next.y / 360.0f) * 360.0f, next.z - floorf(next.z / 360.0f) * 360.0f);

	auto diff = nextEuler - prevEuler;

	auto result = dxm::Vector3::Zero;

	// wrapping X angle
	if (fabsf(diff.x) > 180.0f)
	{
		result.x = diff.x < 0.0f ? prevEuler.x + t * (360.0f - fabsf(diff.x)) : prevEuler.x - t * (360.0f - fabsf(diff.x));
	}
	else
	{
		result.x = prevEuler.x + t * diff.x;
	}

	// wrapping Y angle
	if (fabsf(diff.y) > 180.0f)
	{
		result.y = diff.y < 0.0f ? prevEuler.y + t * (360.0f - fabsf(diff.y)) : prevEuler.y - t * (360.0f - fabsf(diff.y));
	}
	else
	{
		result.y = prevEuler.y + t * diff.y;
	}

	// wrapping Z angle
	if (fabsf(diff.z) > 180.0f)
	{
		result.z = diff.z < 0.0f ? prevEuler.z + t * (360.0f - fabsf(diff.z)) : prevEuler.z - t * (360.0f - fabsf(diff.z));
	}
	else
	{
		result.z = prevEuler.z + t * diff.z;
	}

	return result;
}

auto AnimationClip::InsterpolateQuaternion(const dxm::Quaternion& prev, const dxm::Quaternion& next, float t) -> dxm::Quaternion
{
	return m_SlerpQuaternionInterpolation
		? dxm::Quaternion::Slerp(prev, next, t)
		: dxm::Quaternion::Lerp(prev, next, t);
}
