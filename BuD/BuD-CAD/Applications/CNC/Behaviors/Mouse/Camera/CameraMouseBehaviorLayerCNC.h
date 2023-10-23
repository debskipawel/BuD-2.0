#pragma once

#include <Applications/CNC/Behaviors/Mouse/BaseMouseBehaviorLayerCNC.h>

class CameraMouseBehaviorLayerCNC : public BaseMouseBehaviorLayerCNC
{
public:
	CameraMouseBehaviorLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void OnMiddleButtonDown(int x, int y) override;
	virtual void OnMiddleButtonUp(int x, int y) override;

	virtual void OnScroll(int x, int y, int delta) override;
	virtual void OnMouseMove(int dx, int dy) override;
};
