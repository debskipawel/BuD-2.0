#pragma once

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class ActionMouseBehaviorLayer : public BaseMouseBehaviorLayer
{
public:
	ActionMouseBehaviorLayer(MainViewModel& viewModel);

	virtual void OnLeftButtonDown(int x, int y) override;
	virtual void OnLeftButtonUp(int x, int y) override;

	virtual void OnMouseMove(int dx, int dy) override;

protected:
	virtual void HandleActionStart();
	virtual void HandleActionEnd();

	virtual void HandleActionPerform(int dx, int dy);
};
