#pragma once

#include <BuD.h>

struct KeyFrame
{
public:
	KeyFrame(float timepoint = 0.0f, const dxm::Vector3& position = dxm::Vector3::Zero, const dxm::Vector3& eulerAngles = dxm::Vector3::Zero, const dxm::Quaternion& quaternion = dxm::Quaternion::Identity);
	virtual ~KeyFrame();

	virtual int Id() const { return m_Id; }

	std::string m_Name;

	float m_TimePoint;

	dxm::Vector3 m_Position;
	dxm::Vector3 m_EulerAngles;
	dxm::Quaternion m_Quaternion;

protected:
	int m_Id;

	inline static int s_LastId = 0;
};
