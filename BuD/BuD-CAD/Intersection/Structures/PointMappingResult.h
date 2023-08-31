#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct PointMappingResult
{
	dxm::Vector2 m_Parameter;

	bool m_ResultFound;
};
