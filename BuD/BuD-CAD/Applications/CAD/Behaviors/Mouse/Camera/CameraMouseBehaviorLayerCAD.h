#pragma once

#include <Applications/CAD/Behaviors/Mouse/BaseMouseBehaviorLayerCAD.h>

class CameraMouseBehaviorLayerCAD : public BaseMouseBehaviorLayerCAD
{
public:
	CameraMouseBehaviorLayerCAD(MainDataLayerCAD& dataLayer);

	virtual void OnMiddleButtonDown(int x, int y);
	virtual void OnMiddleButtonUp(int x, int y);

	virtual void OnMouseMove(int dx, int dy);
	virtual void OnScroll(int x, int y, int delta);

protected:
	virtual void HandleCameraRotationEnable();
	virtual void HandleCameraRotationDisable();

	virtual void HandleCameraRotate(int dx, int dy);
	virtual void HandleCameraMove(int dx, int dy);
};
