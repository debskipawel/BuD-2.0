#pragma once

#include <Applications/CNC/DataLayers/MainDataLayerCNC.h>
#include <GUI/BaseGuiLayer.h>

class BaseGuiLayerCNC : public BaseGuiLayer
{
public:
	BaseGuiLayerCNC(MainDataLayerCNC& dataLayer);

protected:
	MainDataLayerCNC& m_MainDataLayer;
};
