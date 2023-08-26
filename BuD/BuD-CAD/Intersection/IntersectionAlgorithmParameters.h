#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct IntersectionAlgorithmParameters
{
	IntersectionAlgorithmParameters()
		: m_PointDistance(0.1f), m_Precision(0.001f), m_UseCursorAsStartingPoint(false)
	{
	}

	dxm::Vector3 m_CursorPosition;

	bool m_UseCursorAsStartingPoint;

	float m_PointDistance;
	float m_Precision;
};
