#include "RayFactory.h"

#include <BuD.h>

RayFactory::RayFactory(ViewportViewModel& viewport, ObjectListViewModel& objectList)
	: m_Viewport(viewport), m_ObjectList(objectList)
{
}

Ray RayFactory::CreateRay(dxm::Vector2 screenSpace)
{
	dxm::Vector4 screenSpacePosition = { screenSpace.x, screenSpace.y, 1.0f, 1.0f };

	auto& scene = m_ObjectList.m_Scene.m_Scene;
	auto camera = scene.ActiveCamera();

	auto invProj = BuD::Renderer::ProjectionMatrix().Invert();
	auto invView = camera->ViewMatrix().Invert();

	auto viewSpacePosition = dxm::Vector4::Transform(screenSpacePosition, invProj);
	viewSpacePosition /= viewSpacePosition.w;

	auto worldSpacePos = dxm::Vector4::Transform(viewSpacePosition, invView);

	Ray ray = {};
	ray.m_Origin = camera->EyePosition();
	ray.m_Direction = worldSpacePos - ray.m_Origin;
	ray.m_Direction.Normalize();

	return ray;
}
