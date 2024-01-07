#pragma once

#include <DataLayers/MainDataLayer.h>

class BaseMouseBehaviorLayer
{
public:
	BaseMouseBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnLeftButtonDown(int x, int y) -> void;
	virtual auto OnMiddleButtonDown(int x, int y) -> void;
	virtual auto OnRightButtonDown(int x, int y) -> void;

	virtual auto OnLeftButtonUp(int x, int y) -> void;
	virtual auto OnMiddleButtonUp(int x, int y) -> void;
	virtual auto OnRightButtonUp(int x, int y) -> void;

	virtual auto OnMove(int x, int y, int dx, int dy) -> void;
	virtual auto OnScroll(int x, int y, int delta) -> void;

protected:
	MainDataLayer& m_MainDataLayer;
};