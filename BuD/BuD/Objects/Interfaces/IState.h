#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class IState
	{
	public:
		virtual ~IState() = default;

	protected:
		dxm::Vector3 m_Position;
		dxm::Vector3 m_Velocity;
		dxm::Quaternion m_Rotation;
		dxm::Vector3 m_AngularVelocity;

		IState() = default;
	};
}
