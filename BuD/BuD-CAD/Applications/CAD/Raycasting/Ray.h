#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct Ray
{
	dxm::Vector3 m_Origin = dxm::Vector3::Zero;
	dxm::Vector3 m_Direction = dxm::Vector3::UnitY;
};
