#pragma once

#include <BuD.h>
#include <ViewModels/MainViewModel.h>

class BaseKeyboardBehaviorLayer
{
public:
	BaseKeyboardBehaviorLayer(MainViewModel& viewModel);

	virtual void OnKeyPress(BuD::KeyboardKeys key);
	virtual void OnKeyRelease(BuD::KeyboardKeys key);

protected:
	MainViewModel& m_ViewModel;
};
