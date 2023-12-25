#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct MaterialBlockDetails
{
	MaterialBlockDetails(const dxm::Vector3& size, const dxm::Vector3& position, float standHeight);

	dxm::Vector3 m_Size;
	dxm::Vector3 m_Position;

	float m_StandHeight;
};
