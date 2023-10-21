#pragma once

#include <BuD.h>

#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>
#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class BaseKeyboardBehaviorLayerCAD : public BaseKeyboardBehaviorLayer
{
public:
	BaseKeyboardBehaviorLayerCAD(MainDataLayerCAD& dataLayer);

protected:
	MainDataLayerCAD& m_MainDataLayer;
};
