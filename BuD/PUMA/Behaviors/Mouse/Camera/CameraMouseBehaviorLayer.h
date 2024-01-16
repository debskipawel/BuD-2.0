#pragma once

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class CameraMouseBehaviorLayer : public BaseMouseBehaviorLayer
{
public:
	CameraMouseBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnMiddleButtonDown(int x, int y) -> void override;
	virtual auto OnMiddleButtonUp(int x, int y) -> void override;

	virtual auto OnMove(int x, int y, int dx, int dy) -> void override;
	virtual auto OnScroll(int x, int y, int delta) -> void override;
};
