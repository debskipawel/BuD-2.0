#include "SimulationDataLayer.h"

SimulationDataLayer::SimulationDataLayer()
	: m_Looped(false), m_Duration(5.0f), m_Running(false), m_Time(0.0f)
{
}

void SimulationDataLayer::Run()
{
	if (m_Time == m_Duration)
	{
		m_Time = 0.0f;
	}

	m_Running = true;
}

void SimulationDataLayer::Stop()
{
	m_Running = false;
}

void SimulationDataLayer::Update(float deltaTime)
{
	if (!m_Running)
	{
		return;
	}

	m_Time += deltaTime;

	if (m_Looped)
	{
		m_Time -= int(m_Time / m_Duration) * m_Duration;
	}
	else if (m_Time >= m_Duration)
	{
		m_Time = m_Duration;
		m_Running = false;
	}
}

KeyFrame SimulationDataLayer::Interpolate()
{
	if (m_KeyFrames.empty())
	{
		return KeyFrame(m_Time);
	}

	auto next = std::upper_bound(m_KeyFrames.begin(), m_KeyFrames.end(), m_Time,
		[](float value, const KeyFrame& keyframe)
		{
			return value < keyframe.m_TimePoint;
		}
	);

	if (next == m_KeyFrames.begin())
	{
		return KeyFrame(m_Time, next->m_Position, next->m_EulerAngles, next->m_Quaternion);
	}

	auto prev = next - 1;

	if (next == m_KeyFrames.end())
	{
		return KeyFrame(m_Time, prev->m_Position, prev->m_EulerAngles, prev->m_Quaternion);
	}

	// ACTUAL INTERPOLATION LOGIC
	// TODO: now Euler angles are interpolated naively, just for sake of benchmark
	auto t = (m_Time - prev->m_TimePoint) / (next->m_TimePoint - prev->m_TimePoint);

	auto position = dxm::Vector3::Lerp(prev->m_Position, next->m_Position, t);
	auto eulerAngles = dxm::Vector3::Lerp(prev->m_EulerAngles, next->m_EulerAngles, t);
	auto quaternion = dxm::Quaternion::Slerp(prev->m_Quaternion, next->m_Quaternion, t);

	return KeyFrame(m_Time, position, eulerAngles, quaternion);
}
