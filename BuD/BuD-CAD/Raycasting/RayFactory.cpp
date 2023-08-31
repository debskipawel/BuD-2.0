#include "RayFactory.h"

#include <BuD.h>

RayFactory::RayFactory(std::shared_ptr<BuD::AbstractCamera> camera)
	: m_Camera(camera)
{
}

Ray RayFactory::CreateRay(dxm::Vector2 screenSpace)
{
	dxm::Vector4 screenSpacePosition = { screenSpace.x, screenSpace.y, 1.0f, 1.0f };

	auto invProj = BuD::Renderer::ProjectionMatrix().Invert();
	auto invView = m_Camera->ViewMatrix().Invert();

	auto viewSpacePosition = dxm::Vector4::Transform(screenSpacePosition, invProj);
	viewSpacePosition /= viewSpacePosition.w;

	auto worldSpacePos = dxm::Vector4::Transform(viewSpacePosition, invView);

	Ray ray = {};
	ray.m_Origin = m_Camera->EyePosition();
	ray.m_Direction = worldSpacePos - ray.m_Origin;
	ray.m_Direction.Normalize();

	return ray;
}
