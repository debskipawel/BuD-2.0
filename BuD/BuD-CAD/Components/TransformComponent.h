#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct TransformComponent
{
	dxm::Vector3 m_Position = dxm::Vector3::Zero;
	dxm::Vector3 m_Scale = dxm::Vector3::One;
	dxm::Vector3 m_Rotation = dxm::Vector3::Zero;
};
