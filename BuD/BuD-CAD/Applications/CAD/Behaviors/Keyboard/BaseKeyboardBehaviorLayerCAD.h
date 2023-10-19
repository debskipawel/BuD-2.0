#pragma once

#include <BuD.h>

#include <Applications/CAD/DataLayers/MainDataLayer.h>
#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class BaseKeyboardBehaviorLayerCAD : public BaseKeyboardBehaviorLayer
{
public:
	BaseKeyboardBehaviorLayerCAD(MainDataLayer& dataLayer);

protected:
	MainDataLayer& m_MainDataLayer;
};
