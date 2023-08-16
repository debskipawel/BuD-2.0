#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class MainKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	MainKeyboardBehaviorLayer(MainDataLayer& dataLayer);

	virtual bool OnKeyPress(BuD::KeyboardKeys key) override;
	virtual bool OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	std::vector<std::unique_ptr<BaseKeyboardBehaviorLayer>> m_BehaviorLayers;
};
