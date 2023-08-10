#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class MultiselectKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	MultiselectKeyboardBehaviorLayer(MainViewModel& viewModel);

	virtual void OnKeyPress(BuD::KeyboardKeys key) override;
	virtual void OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	virtual void HandleMultiselectEnable(BuD::KeyboardKeys key);
	virtual void HandleMultiselectDisable(BuD::KeyboardKeys key);
};
