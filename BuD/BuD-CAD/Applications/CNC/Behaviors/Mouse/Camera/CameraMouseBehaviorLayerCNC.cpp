#include "CameraMouseBehaviorLayerCNC.h"

CameraMouseBehaviorLayerCNC::CameraMouseBehaviorLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseMouseBehaviorLayerCNC(dataLayer)
{
}

void CameraMouseBehaviorLayerCNC::OnMiddleButtonDown(int x, int y)
{
}

void CameraMouseBehaviorLayerCNC::OnMiddleButtonUp(int x, int y)
{
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
}
