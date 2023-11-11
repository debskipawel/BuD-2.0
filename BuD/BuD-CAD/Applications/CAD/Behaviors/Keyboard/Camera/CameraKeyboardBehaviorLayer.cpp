#include "CameraKeyboardBehaviorLayer.h"

CameraKeyboardBehaviorLayer::CameraKeyboardBehaviorLayer(MainDataLayerCAD& dataLayer)
	: BaseKeyboardBehaviorLayerCAD(dataLayer)
{
}

bool CameraKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	auto& appState = m_MainDataLayer.m_AppStateDataLayer;

	if (appState.m_AppState == AppState::FROZEN)
	{
		return false;
	}

	HandleCameraMovementEnable(key);

	return false;
}

bool CameraKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	HandleCameraMovementDisable(key);

	return false;
}

void CameraKeyboardBehaviorLayer::HandleCameraMovementEnable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Shift)
	{
		m_MainDataLayer.m_AppStateDataLayer.m_CameraReadyToMove = true;
	}
}

void CameraKeyboardBehaviorLayer::HandleCameraMovementDisable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Shift)
	{
		m_MainDataLayer.m_AppStateDataLayer.m_CameraReadyToMove = false;
	}
}
