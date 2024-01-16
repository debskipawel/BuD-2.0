#include "PauseKeyboardBehaviorLayer.h"

PauseKeyboardBehaviorLayer::PauseKeyboardBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseKeyboardBehaviorLayer(mainDataLayer)
{
}

auto PauseKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key) -> void
{
	if (key == BuD::KeyboardKeys::Space)
	{
		auto running = m_MainDataLayer.IsRunning();

		running ? m_MainDataLayer.Stop() : m_MainDataLayer.Start();
	}
}
