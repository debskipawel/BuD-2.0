#pragma once

#include <KeyFrame.h>

class AnimationClip
{
public:
	AnimationClip(const std::vector<KeyFrame>& keyFrames, float duration);

	virtual auto AddKeyFrame(float time) -> void;
	virtual auto AddKeyFrame(const KeyFrame& keyFrame) -> void;

	virtual auto RemoveKeyFrame(unsigned int id) -> bool;
	virtual auto RemoveKeyFrame(const KeyFrame& keyFrame) -> bool;

	virtual auto GetKeyFrames() const -> const std::vector<KeyFrame>&;
	virtual auto GetIntermediateFrames(unsigned int frameCount) -> std::vector<KeyFrame>;

	virtual auto GetKeyFrame(unsigned int id) -> std::optional<KeyFrame*>;
	virtual auto SortKeyFrames() -> void;

	virtual auto Interpolate(float time) -> KeyFrame;

	constexpr virtual auto GetDuration() const -> float;
	virtual auto SetDuration(float duration) -> void;

	bool m_SlerpQuaternionInterpolation;

protected:
	virtual auto InterpolateEuler(const dxm::Vector3& prev, const dxm::Vector3& next, float t) -> dxm::Vector3;
	virtual auto InterpolateQuaternion(const dxm::Quaternion& prev, const dxm::Quaternion& next, float t) -> dxm::Quaternion;

	std::vector<KeyFrame> m_KeyFrames;
	float m_Duration;
};
