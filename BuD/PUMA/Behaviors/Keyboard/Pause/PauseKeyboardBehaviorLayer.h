#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class PauseKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	PauseKeyboardBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnKeyPress(BuD::KeyboardKeys key) -> void override;
};
