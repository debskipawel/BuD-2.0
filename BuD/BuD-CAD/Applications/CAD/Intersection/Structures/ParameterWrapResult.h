#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct ParameterWrapResult
{
	dxm::Vector4 m_Parameter;

	bool m_OutOfBounds;
	float m_WrappedU, m_WrappedV, m_WrappedS, m_WrappedT;
};
