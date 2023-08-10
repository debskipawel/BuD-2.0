#include "MultiselectKeyboardBehaviorLayer.h"

MultiselectKeyboardBehaviorLayer::MultiselectKeyboardBehaviorLayer(MainViewModel& viewModel)
	: BaseKeyboardBehaviorLayer(viewModel)
{
}

void MultiselectKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	auto& appStateVM = m_ViewModel.m_AppStateViewModel;

	if (appStateVM.m_AppState == AppState::FROZEN)
	{
		return;
	}

	HandleMultiselectEnable(key);
}

void MultiselectKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	HandleMultiselectDisable(key);
}

void MultiselectKeyboardBehaviorLayer::HandleMultiselectEnable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Control)
	{
		m_ViewModel.m_AppStateViewModel.m_MultiselectOn = true;
	}
}

void MultiselectKeyboardBehaviorLayer::HandleMultiselectDisable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Control)
	{
		m_ViewModel.m_AppStateViewModel.m_MultiselectOn = false;
	}
}
