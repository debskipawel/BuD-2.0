#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class MultiselectKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	MultiselectKeyboardBehaviorLayer(MainDataLayer& dataLayer);

	virtual bool OnKeyPress(BuD::KeyboardKeys key) override;
	virtual bool OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	virtual void HandleMultiselectEnable(BuD::KeyboardKeys key);
	virtual void HandleMultiselectDisable(BuD::KeyboardKeys key);
};
