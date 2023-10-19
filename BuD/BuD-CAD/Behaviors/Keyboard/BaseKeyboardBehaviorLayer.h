#pragma once

#include <BuD.h>

class BaseKeyboardBehaviorLayer
{
public:
	virtual ~BaseKeyboardBehaviorLayer() = default;

	virtual bool OnKeyPress(BuD::KeyboardKeys key);
	virtual bool OnKeyRelease(BuD::KeyboardKeys key);
};
