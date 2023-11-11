#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct NextCommonPointResult
{
	dxm::Vector4 m_Parameter;
	dxm::Vector4 m_Step;
	dxm::Vector3 m_Point;

	float m_WrappedU, m_WrappedV, m_WrappedS, m_WrappedT;

	bool m_ResultFound;
	bool m_ShouldContinue;
};
