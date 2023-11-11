#pragma once

#include <Applications/Switcher/DataLayers/MainDataLayerSwitcher.h>
#include <Applications/Shared/GUI/BaseGuiLayer.h>

class BaseGuiLayerSwitcher : public BaseGuiLayer
{
public:
	BaseGuiLayerSwitcher(MainDataLayerSwitcher& dataLayer);

protected:
	MainDataLayerSwitcher& m_MainDataLayer;
};
