#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class MainKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	MainKeyboardBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnKeyPress(BuD::KeyboardKeys key) -> void override;
	virtual auto OnKeyRelease(BuD::KeyboardKeys key) -> void override;

protected:
	std::vector<std::unique_ptr<BaseKeyboardBehaviorLayer>> m_BehaviorLayers;
};
