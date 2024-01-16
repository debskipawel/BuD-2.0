#include "BaseKeyboardBehaviorLayer.h"

BaseKeyboardBehaviorLayer::BaseKeyboardBehaviorLayer(MainDataLayer& mainDataLayer)
	: m_MainDataLayer(mainDataLayer)
{
}

auto BaseKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key) -> void
{
}

auto BaseKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key) -> void
{
}
