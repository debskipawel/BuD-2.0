#pragma once

#include <SimpleMath.h>

namespace BuD
{
	struct AABB
	{
		DirectX::SimpleMath::Vector3 m_Min;
		DirectX::SimpleMath::Vector3 m_Max;
	};
}
