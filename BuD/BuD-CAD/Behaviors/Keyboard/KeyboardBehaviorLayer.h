#pragma once

#include <BuD.h>
#include <ViewModels/MainViewModel.h>

class KeyboardBehaviorLayer
{
public:
	KeyboardBehaviorLayer(MainViewModel& viewModel);

	virtual void OnKeyPress(BuD::KeyboardKeys key);
	virtual void OnKeyRelease(BuD::KeyboardKeys key);

protected:
	virtual void HandleMultiselectEnable(BuD::KeyboardKeys key);
	virtual void HandleMultiselectDisable(BuD::KeyboardKeys key);

	virtual void HandleCameraMovementEnable(BuD::KeyboardKeys key);
	virtual void HandleCameraMovementDisable(BuD::KeyboardKeys key);

	virtual void HandleAppStateChange(BuD::KeyboardKeys key);
	virtual void HandleAxisLockEnable(BuD::KeyboardKeys key);
	virtual void HandleAxisLockDisable(BuD::KeyboardKeys key);

	void SetAxisLock(AxisLock axisLock);

	MainViewModel& m_ViewModel;
};
