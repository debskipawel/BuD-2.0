#pragma once

#include <BuD.h>
#include <DataLayers/MainDataLayer.h>

class BaseKeyboardBehaviorLayer
{
public:
	BaseKeyboardBehaviorLayer(MainDataLayer& dataLayer);
	virtual ~BaseKeyboardBehaviorLayer() = default;

	virtual void OnKeyPress(BuD::KeyboardKeys key);
	virtual void OnKeyRelease(BuD::KeyboardKeys key);

protected:
	MainDataLayer& m_MainDataLayer;
};
