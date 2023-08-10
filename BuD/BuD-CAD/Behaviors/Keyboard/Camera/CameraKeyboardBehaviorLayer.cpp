#include "CameraKeyboardBehaviorLayer.h"

CameraKeyboardBehaviorLayer::CameraKeyboardBehaviorLayer(MainViewModel& viewModel)
	: BaseKeyboardBehaviorLayer(viewModel)
{
}

void CameraKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	auto& appStateVM = m_ViewModel.m_AppStateViewModel;

	if (appStateVM.m_AppState == AppState::FROZEN)
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
		m_ViewModel.m_AppStateViewModel.m_CameraReadyToMove = true;
	}
}

void CameraKeyboardBehaviorLayer::HandleCameraMovementDisable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Shift)
	{
		m_ViewModel.m_AppStateViewModel.m_CameraReadyToMove = false;
	}
}
