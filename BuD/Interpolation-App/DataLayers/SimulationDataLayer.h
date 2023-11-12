#pragma once

#include <BuD.h>

struct KeyFrame
{
public:
	KeyFrame(float timepoint = 0.0f, const dxm::Vector3& position = dxm::Vector3::Zero, const dxm::Vector3& eulerAngles = dxm::Vector3::Zero, const dxm::Quaternion& quaternion = dxm::Quaternion::Identity);

	float m_TimePoint;

	dxm::Vector3 m_Position;
	dxm::Vector3 m_EulerAngles;
	dxm::Quaternion m_Quaternion;
};

struct SimulationDataLayer
{
	SimulationDataLayer();

	virtual void Run();
	virtual void Stop();

	virtual void Update(float deltaTime);

	bool m_Running;
	bool m_Looped;

	float m_Duration;
	float m_Time;

	std::vector<KeyFrame> m_KeyFrames;
};
