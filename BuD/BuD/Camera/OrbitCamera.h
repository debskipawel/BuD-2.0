#pragma once

#include "AbstractCamera.h"

namespace BuD
{
	class OrbitCamera : public AbstractCamera
	{
	public:
		explicit OrbitCamera(dxm::Vector3 target = dxm::Vector3(0, 0, 0),
			float minDistance = 0.0f, float maxDistance = FLT_MAX, float distance = 0.0f);
		explicit OrbitCamera(float minDistance, float maxDistance = FLT_MAX, float distance = 0.0f);

		virtual dxm::Matrix ViewMatrix() override;
		
		virtual dxm::Vector3 EyePosition() override;

		virtual dxm::Vector3 Front() override;
		virtual dxm::Vector3 Right() override;
		virtual dxm::Vector3 Up() override;

		virtual void RotateCamera(float dx, float dy) override;
		virtual void Zoom(float dd) override;
		
		virtual void Move(dxm::Vector3 difference) override;
		virtual void MoveTo(dxm::Vector3 position) override;

	protected:
		void ClampDistance();

		dxm::Vector3 m_Target;

		float m_AngleX, m_AngleY;
		float m_Distance, m_MinDistance, m_MaxDistance;
	};
}
