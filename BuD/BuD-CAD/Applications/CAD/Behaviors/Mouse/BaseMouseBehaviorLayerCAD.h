#pragma once

#include <Applications/CAD/DataLayers/MainDataLayer.h>
#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class BaseMouseBehaviorLayerCAD : public BaseMouseBehaviorLayer
{
public:
	BaseMouseBehaviorLayerCAD(MainDataLayer& dataLayer);

protected:
	virtual dxm::Vector2 ViewportScreenSpaceCoords(int x, int y);

	MainDataLayer& m_MainDataLayer;
};
