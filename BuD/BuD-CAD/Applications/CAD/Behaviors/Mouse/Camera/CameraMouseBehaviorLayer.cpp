#include "CameraMouseBehaviorLayer.h"

CameraMouseBehaviorLayer::CameraMouseBehaviorLayer(MainDataLayer& dataLayer)
	: BaseMouseBehaviorLayerCAD(dataLayer)
{
}

void CameraMouseBehaviorLayer::OnMiddleButtonDown(int x, int y)
{
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return;
	}

	HandleCameraRotationEnable();
}

void CameraMouseBehaviorLayer::OnMiddleButtonUp(int x, int y)
{
	HandleCameraRotationDisable();
}

void CameraMouseBehaviorLayer::OnMouseMove(int dx, int dy)
{
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_MainDataLayer.m_AppStateDataLayer.m_CameraRotating)
	{
		if (m_MainDataLayer.m_AppStateDataLayer.m_CameraReadyToMove)
		{
			HandleCameraMove(dx, dy);
		}
		else
		{
			HandleCameraRotate(dx, dy);
		}
	}
}

void CameraMouseBehaviorLayer::OnScroll(int x, int y, int delta)
{
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_MainDataLayer.m_ViewportDataLayer.IsMouseOnViewport(x, y))
	{
		auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_Scene;
		auto camera = scene.ActiveCamera();

		camera->Zoom(-0.01f * delta);
	}
}

void CameraMouseBehaviorLayer::HandleCameraRotationEnable()
{
	m_MainDataLayer.m_AppStateDataLayer.m_CameraRotating = true;
}

void CameraMouseBehaviorLayer::HandleCameraRotationDisable()
{
	m_MainDataLayer.m_AppStateDataLayer.m_CameraRotating = false;
}

void CameraMouseBehaviorLayer::HandleCameraRotate(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_Scene;
	auto camera = scene.ActiveCamera();

	camera->RotateCamera(0.005 * dx, 0.005 * dy);
}

void CameraMouseBehaviorLayer::HandleCameraMove(int dx, int dy)
{
	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_Scene;
	auto camera = scene.ActiveCamera();

	auto right = camera->Right();
	auto up = camera->Up();

	auto direction = -0.05f * (dx * right - dy * up);

	camera->Move(direction);
}
