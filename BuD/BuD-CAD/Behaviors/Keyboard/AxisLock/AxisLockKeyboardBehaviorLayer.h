#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class AxisLockKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	AxisLockKeyboardBehaviorLayer(MainViewModel& viewModel);

	virtual void OnKeyPress(BuD::KeyboardKeys key) override;
	virtual void OnKeyRelease(BuD::KeyboardKeys key) override;

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
