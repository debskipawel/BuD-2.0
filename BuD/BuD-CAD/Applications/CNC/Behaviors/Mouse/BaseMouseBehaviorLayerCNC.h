#pragma once

#include <Applications/CNC/DataLayers/MainDataLayerCNC.h>
#include <Applications/Shared/Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class BaseMouseBehaviorLayerCNC : public BaseMouseBehaviorLayer
{
public:
	BaseMouseBehaviorLayerCNC(MainDataLayerCNC& dataLayer);

protected:
	MainDataLayerCNC& m_MainDataLayer;
};
