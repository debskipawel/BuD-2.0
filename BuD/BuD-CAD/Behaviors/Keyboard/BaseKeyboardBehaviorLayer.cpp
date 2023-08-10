#include "BaseKeyboardBehaviorLayer.h"

BaseKeyboardBehaviorLayer::BaseKeyboardBehaviorLayer(MainViewModel& viewModel)
	: m_ViewModel(viewModel)
{
}

void BaseKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
}

void BaseKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
}
