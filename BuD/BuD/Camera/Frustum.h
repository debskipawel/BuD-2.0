#pragma once

#include <array>

#include <Objects/AABB.h>

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

namespace BuD
{
	class Frustum
	{
	public:
		Frustum(dxm::Vector3 viewPosition, dxm::Vector3 front, dxm::Vector3 right, float aspectRatio, float fov, float nearPlane, float farPlane);

		bool IsInside(const AABB& boundingBox) const;

	private:
		struct Plane
		{
			dxm::Vector3 m_PointOnPlane;
			dxm::Vector3 m_NormalToPlane;
		};

		std::array<Plane, 6> m_FrustumPlanes;
	};
}
