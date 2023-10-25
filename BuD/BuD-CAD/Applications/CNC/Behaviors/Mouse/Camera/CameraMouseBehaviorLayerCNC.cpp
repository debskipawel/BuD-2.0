#include "CameraMouseBehaviorLayerCNC.h"

CameraMouseBehaviorLayerCNC::CameraMouseBehaviorLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseMouseBehaviorLayerCNC(dataLayer)
{
}

void CameraMouseBehaviorLayerCNC::OnMiddleButtonDown(int x, int y)
{
	if (m_MainDataLayer.m_ViewportDataLayer.IsMouseOnViewport(x, y))
	{
		m_MainDataLayer.m_CameraDataLayer.m_CameraInMove = true;
	}
}

void CameraMouseBehaviorLayerCNC::OnMiddleButtonUp(int x, int y)
{
	m_MainDataLayer.m_CameraDataLayer.m_CameraInMove = false;
}

void CameraMouseBehaviorLayerCNC::OnScroll(int x, int y, int delta)
{
	if (m_MainDataLayer.m_ViewportDataLayer.IsMouseOnViewport(x, y))
	{
		auto& scene = m_MainDataLayer.m_SceneDataLayer.m_Scene;
		auto camera = scene.ActiveCamera();

		camera->Zoom(-0.01f * delta);
	}
}

void CameraMouseBehaviorLayerCNC::OnMouseMove(int dx, int dy)
{
	if (m_MainDataLayer.m_CameraDataLayer.m_CameraInMove)
	{
		auto& scene = m_MainDataLayer.m_SceneDataLayer.m_Scene;
		auto camera = scene.ActiveCamera();

		camera->RotateCamera(0.005 * dx, 0.005 * dy);
	}
}
