#pragma once

#include <DataLayers/MainDataLayer.h>

class BaseKeyboardBehaviorLayer
{
public:
	BaseKeyboardBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnKeyPress(BuD::KeyboardKeys key) -> void;
	virtual auto OnKeyRelease(BuD::KeyboardKeys key) -> void;

protected:
	MainDataLayer& m_MainDataLayer;
};
