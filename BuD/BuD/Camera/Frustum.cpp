#include "bud_pch.h"
#include "Frustum.h"

#include <numbers>

namespace BuD
{
	Frustum::Frustum(dxm::Vector3 viewPosition, dxm::Vector3 front, dxm::Vector3 right, float aspectRatio, float fov, float nearPlane, float farPlane)
	{
		front.Normalize();
		right.Normalize();

		auto up = right.Cross(front);
		up.Normalize();

		auto& backPlane = m_FrustumPlanes[0];
		backPlane.m_PointOnPlane = viewPosition + nearPlane * front;
		backPlane.m_NormalToPlane = -front;

		auto& frontPlane = m_FrustumPlanes[1];
		frontPlane.m_PointOnPlane = viewPosition + farPlane * front;
		frontPlane.m_NormalToPlane = front;

		auto& leftPlane = m_FrustumPlanes[2];
		auto& rightPlane = m_FrustumPlanes[3];

		leftPlane.m_PointOnPlane = rightPlane.m_PointOnPlane = viewPosition;

		auto verticalAngle = fov * 0.5f;
		auto horizontalAngle = verticalAngle * aspectRatio;

		auto rightAngle = (horizontalAngle + 90.0f) * std::numbers::pi / 180;
		auto leftAngle = -rightAngle;

		leftPlane.m_NormalToPlane = cos(leftAngle) * front + sin(leftAngle) * right;
		rightPlane.m_NormalToPlane = cos(rightAngle) * front + sin(rightAngle) * right;

		auto& upPlane = m_FrustumPlanes[4];
		auto& downPlane = m_FrustumPlanes[5];

		upPlane.m_PointOnPlane = downPlane.m_PointOnPlane = viewPosition;

		auto upAngle = (verticalAngle + 90.0f) * std::numbers::pi / 180;
		auto downAngle = -upAngle;

		upPlane.m_NormalToPlane = cos(upAngle) * front + sin(upAngle) * up;
		downPlane.m_NormalToPlane = cos(downAngle) * front + sin(downAngle) * up;
	}

	bool Frustum::IsInside(const AABB& boundingBox) const
	{
		for (auto& plane : m_FrustumPlanes)
		{
			auto v1 = boundingBox.m_Min - plane.m_PointOnPlane;
			auto v2 = boundingBox.m_Max - plane.m_PointOnPlane;

			if (v1.Dot(plane.m_NormalToPlane) > 0 && v2.Dot(plane.m_NormalToPlane) > 0)
			{
				return false;
			}
		}

		return true;
	}
}
