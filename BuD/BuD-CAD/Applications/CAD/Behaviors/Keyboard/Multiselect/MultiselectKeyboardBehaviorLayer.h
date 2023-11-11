#pragma once

#include <Applications/CAD/Behaviors/Keyboard/BaseKeyboardBehaviorLayerCAD.h>

class MultiselectKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayerCAD
{
public:
	MultiselectKeyboardBehaviorLayer(MainDataLayerCAD& dataLayer);

	virtual bool OnKeyPress(BuD::KeyboardKeys key) override;
	virtual bool OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	virtual void HandleMultiselectEnable(BuD::KeyboardKeys key);
	virtual void HandleMultiselectDisable(BuD::KeyboardKeys key);
};
