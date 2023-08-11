#include "CameraKeyboardBehaviorLayer.h"

CameraKeyboardBehaviorLayer::CameraKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayer(dataLayer)
{
}

void CameraKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	auto& appState = m_MainDataLayer.m_AppStateDataLayer;

	if (appState.m_AppState == AppState::FROZEN)
	{
		return;
	}

	HandleCameraMovementEnable(key);
}

void CameraKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	HandleCameraMovementDisable(key);
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
