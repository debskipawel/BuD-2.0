#include "MultiselectKeyboardBehaviorLayer.h"

MultiselectKeyboardBehaviorLayer::MultiselectKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayerCAD(dataLayer)
{
}

bool MultiselectKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	auto& appState = m_MainDataLayer.m_AppStateDataLayer;

	if (appState.m_AppState == AppState::FROZEN)
	{
		return false;
	}

	HandleMultiselectEnable(key);

	return false;
}

bool MultiselectKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	HandleMultiselectDisable(key);

	return false;
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
