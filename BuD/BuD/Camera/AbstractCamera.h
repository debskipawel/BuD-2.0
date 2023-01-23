#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class AbstractCamera
	{
	public:
		virtual dxm::Matrix ViewMatrix() = 0;

		virtual dxm::Vector3 EyePosition() = 0;

		virtual dxm::Vector3 Front() = 0;
		virtual dxm::Vector3 Right() = 0;
		virtual dxm::Vector3 Up() = 0;

		virtual void RotateCamera(float dx, float dy) = 0;
		virtual void Zoom(float dd) = 0;

		virtual void Move(dxm::Vector3 difference) = 0;
		virtual void MoveTo(dxm::Vector3 position) = 0;
	};
}
