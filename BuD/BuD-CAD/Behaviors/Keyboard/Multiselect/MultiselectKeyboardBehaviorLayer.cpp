#include "MultiselectKeyboardBehaviorLayer.h"

MultiselectKeyboardBehaviorLayer::MultiselectKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayer(dataLayer)
{
}

void MultiselectKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	auto& appState = m_MainDataLayer.m_AppStateDataLayer;

	if (appState.m_AppState == AppState::FROZEN)
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
		m_MainDataLayer.m_AppStateDataLayer.m_MultiselectOn = true;
	}
}

void MultiselectKeyboardBehaviorLayer::HandleMultiselectDisable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Control)
	{
		m_MainDataLayer.m_AppStateDataLayer.m_MultiselectOn = false;
	}
}
