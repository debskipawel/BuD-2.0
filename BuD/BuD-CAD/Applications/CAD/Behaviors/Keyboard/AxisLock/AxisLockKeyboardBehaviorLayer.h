#pragma once

#include <Applications/CAD/Behaviors/Keyboard/BaseKeyboardBehaviorLayerCAD.h>

class AxisLockKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayerCAD
{
public:
	AxisLockKeyboardBehaviorLayer(MainDataLayer& dataLayer);

	virtual bool OnKeyPress(BuD::KeyboardKeys key) override;
	virtual bool OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	virtual void HandleAxisLockEnable(BuD::KeyboardKeys key);
	virtual void HandleAxisLockDisable(BuD::KeyboardKeys key);

	void SetAxisLock(AxisLock axisLock);

	inline static std::map<BuD::KeyboardKeys, AxisLock> s_AxisLockMap =
	{
		{ BuD::KeyboardKeys::X, AxisLock::X },
		{ BuD::KeyboardKeys::Y, AxisLock::Y },
		{ BuD::KeyboardKeys::Z, AxisLock::Z },
	};

	inline static std::stack<AxisLock> s_AxisLockStack = {};
};
