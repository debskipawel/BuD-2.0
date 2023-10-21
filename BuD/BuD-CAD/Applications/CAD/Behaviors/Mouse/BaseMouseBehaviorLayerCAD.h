#pragma once

#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>
#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class BaseMouseBehaviorLayerCAD : public BaseMouseBehaviorLayer
{
public:
	BaseMouseBehaviorLayerCAD(MainDataLayerCAD& dataLayer);

protected:
	virtual dxm::Vector2 ViewportScreenSpaceCoords(int x, int y);

	MainDataLayerCAD& m_MainDataLayer;
};
