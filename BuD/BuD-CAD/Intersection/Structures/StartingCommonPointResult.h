#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct StartingCommonPointResult
{
	dxm::Vector4 m_Parameter;
	dxm::Vector3 m_Point;

	int m_IterationCount;
	bool m_ResultFound;
};
