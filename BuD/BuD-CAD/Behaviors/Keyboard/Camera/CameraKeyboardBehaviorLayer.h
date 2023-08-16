#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class CameraKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	CameraKeyboardBehaviorLayer(MainDataLayer& dataLayer);

	virtual bool OnKeyPress(BuD::KeyboardKeys key) override;
	virtual bool OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	virtual void HandleCameraMovementEnable(BuD::KeyboardKeys key);
	virtual void HandleCameraMovementDisable(BuD::KeyboardKeys key);
};
