#pragma once

#include <DataLayers/MainDataLayer.h>

class BaseMouseBehaviorLayer
{
public:
	BaseMouseBehaviorLayer(MainDataLayer& dataLayer);

	virtual void OnLeftButtonDown(int x, int y) {}
	virtual void OnRightButtonDown(int x, int y) {}
	virtual void OnMiddleButtonDown(int x, int y) {}

	virtual void OnLeftButtonUp(int x, int y) {}
	virtual void OnRightButtonUp(int x, int y) {}
	virtual void OnMiddleButtonUp(int x, int y) {}

	virtual void OnScroll(int x, int y, int delta) {}
	virtual void OnMouseMove(int dx, int dy) {}

protected:
	virtual dxm::Vector2 ViewportScreenSpaceCoords(int x, int y);

	MainDataLayer& m_MainDataLayer;
};
