#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class IState
	{
	public:
		virtual ~IState() = default;

		dxm::Vector3 Position = dxm::Vector3::Zero;
		dxm::Vector3 Velocity = dxm::Vector3::Zero;
		dxm::Quaternion Rotation = dxm::Quaternion::Identity;
		dxm::Vector3 AngularVelocity = dxm::Vector3::Zero;

		IState() = default;
	};
}
