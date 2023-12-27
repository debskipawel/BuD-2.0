#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class CameraKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	CameraKeyboardBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnKeyPress(BuD::KeyboardKeys key) -> void override;
	virtual auto OnKeyRelease(BuD::KeyboardKeys key) -> void override;
};
