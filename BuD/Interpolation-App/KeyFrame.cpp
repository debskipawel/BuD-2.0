#include "KeyFrame.h"

KeyFrame::KeyFrame(float timepoint, const dxm::Vector3& position, const dxm::Vector3& eulerAngles, const dxm::Quaternion& quaternion)
	: m_TimePoint(timepoint), m_Position(position), m_EulerAngles(eulerAngles), m_Quaternion(quaternion)
{
	m_Id = s_LastId++;
}

KeyFrame::~KeyFrame()
{
}
