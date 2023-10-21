#pragma once

#include <Applications/CAD/Behaviors/Keyboard/BaseKeyboardBehaviorLayerCAD.h>

class MainKeyboardBehaviorLayerCAD : public BaseKeyboardBehaviorLayerCAD
{
public:
	MainKeyboardBehaviorLayerCAD(MainDataLayerCAD& dataLayer);

	virtual bool OnKeyPress(BuD::KeyboardKeys key) override;
	virtual bool OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	std::vector<std::unique_ptr<BaseKeyboardBehaviorLayer>> m_BehaviorLayers;
};
