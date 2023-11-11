#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct IntersectionPoint
{
	IntersectionPoint(dxm::Vector2 parameter, float wrappedU, float wrappedV)
		: m_Parameter(parameter), m_WrappedU(wrappedU), m_WrappedV(wrappedV)
	{
	}

	dxm::Vector2 m_Parameter;

	float m_WrappedU, m_WrappedV;
};
