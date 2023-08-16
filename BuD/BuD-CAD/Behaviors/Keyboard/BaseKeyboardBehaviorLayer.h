#pragma once

#include <BuD.h>
#include <DataLayers/MainDataLayer.h>

class BaseKeyboardBehaviorLayer
{
public:
	BaseKeyboardBehaviorLayer(MainDataLayer& dataLayer);
	virtual ~BaseKeyboardBehaviorLayer() = default;

	virtual bool OnKeyPress(BuD::KeyboardKeys key);
	virtual bool OnKeyRelease(BuD::KeyboardKeys key);

protected:
	MainDataLayer& m_MainDataLayer;
};
