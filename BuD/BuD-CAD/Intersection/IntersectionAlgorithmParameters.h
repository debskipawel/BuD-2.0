#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct IntersectionAlgorithmParameters
{
	dxm::Vector4 m_StartingParameters;
	bool m_UseSelectedStartingPoint;

	float m_Precision;
};
