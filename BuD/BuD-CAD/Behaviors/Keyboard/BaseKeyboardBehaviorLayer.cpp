#include "BaseKeyboardBehaviorLayer.h"

BaseKeyboardBehaviorLayer::BaseKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: m_MainDataLayer(dataLayer)
{
}

bool BaseKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	return false;
}

bool BaseKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	return false;
}
