#include "RayIntersectionVisitor.h"

RayIntersectionVisitor::RayIntersectionVisitor(const Ray& ray)
	: m_Ray(ray)
{
}

void RayIntersectionVisitor::Visit(SceneObjectCAD& object)
{
	m_LatestResults = {};

	AbstractVisitor::Visit(object);
}

void RayIntersectionVisitor::Visit(Torus& torus)
{
	auto& instanceData = torus.m_InstanceData;
	auto modelMatrix = instanceData.m_ModelMatrix;

	if (abs(modelMatrix.Determinant()) < FLT_EPSILON)
	{
		// in such case it's either a plane (if scale 0 in 1 dim), a line (if scale 0 in 2 dim), or a point (if scale 0 in all dims)
		// it could be useful to accomodate for such cases, as it should be easier than the actual intersection logic
		return;
	}

	auto invModelMatrix = modelMatrix.Invert();

	auto origin = dxm::Vector4(m_Ray.m_Origin.x, m_Ray.m_Origin.y, m_Ray.m_Origin.z, 1.0f);
	auto direction = dxm::Vector4(m_Ray.m_Direction.x, m_Ray.m_Direction.y, m_Ray.m_Direction.z, 0.0f);

	auto localOrigin = dxm::Vector4::Transform(origin, invModelMatrix);
	auto localDirection = dxm::Vector4::Transform(direction, invModelMatrix);

	Ray localRay = {};
	localRay.m_Origin = dxm::Vector3(localOrigin);
	localRay.m_Direction = dxm::Vector3(localDirection);
	localRay.m_Direction.Normalize();

	// Bounding volume test - if not intersecting the bounding sphere, then surely not intersecting 
	auto shortestSphereToRayDistance = localRay.m_Direction.Dot(-localRay.m_Origin) / localRay.m_Direction.Dot(localRay.m_Direction);
	auto shortestSphereToRay = localRay.m_Origin + shortestSphereToRayDistance * localRay.m_Direction;

	if (shortestSphereToRay.LengthSquared() > pow(instanceData.m_InnerRadius + instanceData.m_OuterRadius, 2.0f))
	{
		m_LatestResults.m_Hit = false;
		m_LatestResults.m_Distance = 0.0f;
		return;
	}

	// TODO: finish logic for intersection with torus
	m_LatestResults.m_Hit = true;
	return;

	auto xa = localRay.m_Origin;
	auto s = localRay.m_Direction;

	auto alpha = s.Dot(s);
	auto beta = s.Dot(xa);
	auto gamma = xa.Dot(xa);
	auto delta = gamma + pow(instanceData.m_OuterRadius, 2.0f) - pow(instanceData.m_InnerRadius, 2.0f);
	auto sigma = gamma - pow(instanceData.m_OuterRadius, 2.0f) - pow(instanceData.m_InnerRadius, 2.0f);

	auto a = alpha * alpha;
	auto b = 4 * alpha * beta;
}

void RayIntersectionVisitor::Visit(Cube& cube)
{
}
