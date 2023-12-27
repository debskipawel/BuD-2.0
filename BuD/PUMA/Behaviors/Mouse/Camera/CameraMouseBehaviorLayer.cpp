#include "CameraMouseBehaviorLayer.h"

CameraMouseBehaviorLayer::CameraMouseBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseMouseBehaviorLayer(mainDataLayer)
{
}

auto CameraMouseBehaviorLayer::OnMiddleButtonDown(int x, int y) -> void
{
	auto& viewport = m_MainDataLayer.m_ViewportDataLayer;

	if (!viewport.IsMouseOnViewport(x, y))
	{
		return;
	}

	m_MainDataLayer.m_CameraDataLayer.m_RotateCamera = true;
}

auto CameraMouseBehaviorLayer::OnMiddleButtonUp(int x, int y) -> void
{
	auto& viewport = m_MainDataLayer.m_ViewportDataLayer;

	if (!viewport.IsMouseOnViewport(x, y))
	{
		return;
	}

	m_MainDataLayer.m_CameraDataLayer.m_RotateCamera = false;
}

auto CameraMouseBehaviorLayer::OnMove(int x, int y, int dx, int dy) -> void
{
	if (!m_MainDataLayer.m_CameraDataLayer.m_RotateCamera)
	{
		return;
	}
	
	auto& scene = m_MainDataLayer.m_Scene;

	auto camera = scene.ActiveCamera();

	camera->RotateCamera(0.01f * dx, 0.01f * dy);
}

auto CameraMouseBehaviorLayer::OnScroll(int x, int y, int delta) -> void
{
	auto& viewport = m_MainDataLayer.m_ViewportDataLayer;

	if (!viewport.IsMouseOnViewport(x, y))
	{
		return;
	}

	auto& scene = m_MainDataLayer.m_Scene;

	auto camera = scene.ActiveCamera();

	camera->Zoom(-0.01f * delta);
}
