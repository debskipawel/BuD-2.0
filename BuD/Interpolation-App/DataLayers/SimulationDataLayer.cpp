#include "SimulationDataLayer.h"

KeyFrame::KeyFrame(float timepoint, const dxm::Vector3& position, const dxm::Vector3& eulerAngles, const dxm::Quaternion& quaternion)
	: m_TimePoint(timepoint), m_Position(position), m_EulerAngles(eulerAngles), m_Quaternion(quaternion)
{
}

SimulationDataLayer::SimulationDataLayer()
	: m_Looped(false), m_Duration(5.0f), m_Running(false), m_Time(0.0f)
{
	// TODO: remove- just for benchmark
	m_KeyFrames = { KeyFrame(0.0f), KeyFrame(1.0f), KeyFrame(3.0f), KeyFrame(4.0f), KeyFrame(5.0f) };
}

void SimulationDataLayer::Run()
{
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

	std::sort(m_KeyFrames.begin(), m_KeyFrames.end(), 
		[](const KeyFrame& frame1, const KeyFrame& frame2)
		{
			return frame1.m_TimePoint > frame2.m_TimePoint;
		}
	);

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
