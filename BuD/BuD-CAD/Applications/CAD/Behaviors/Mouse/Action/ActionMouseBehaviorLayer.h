#pragma once

#include <Applications/CAD/Behaviors/Mouse/BaseMouseBehaviorLayerCAD.h>

class ActionMouseBehaviorLayer : public BaseMouseBehaviorLayerCAD
{
public:
	ActionMouseBehaviorLayer(MainDataLayer& dataLayer);

	virtual void OnLeftButtonDown(int x, int y) override;
	virtual void OnLeftButtonUp(int x, int y) override;

	virtual void OnMouseMove(int dx, int dy) override;

protected:
	virtual void HandleActionStart();
	virtual void HandleActionEnd();

	virtual void HandleActionPerform(int dx, int dy);

	virtual void Move(int dx, int dy);
	virtual void Rotate(int dx, int dy);
	virtual void Scale(int dx, int dy);

	virtual void ApplyGroupMouseAction();
};
