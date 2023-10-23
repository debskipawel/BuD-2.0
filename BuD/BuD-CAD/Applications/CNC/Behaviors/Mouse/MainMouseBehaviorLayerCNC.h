#pragma once

#include <Applications/CNC/Behaviors/Mouse/BaseMouseBehaviorLayerCNC.h>

class MainMouseBehaviorLayerCNC : public BaseMouseBehaviorLayerCNC
{
public:
	MainMouseBehaviorLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void OnLeftButtonDown(int x, int y) override;
	virtual void OnRightButtonDown(int x, int y) override;
	virtual void OnMiddleButtonDown(int x, int y) override;

	virtual void OnLeftButtonUp(int x, int y) override;
	virtual void OnRightButtonUp(int x, int y) override;
	virtual void OnMiddleButtonUp(int x, int y) override;

	virtual void OnScroll(int x, int y, int delta) override;
	virtual void OnMouseMove(int dx, int dy) override;

protected:
	std::vector<std::unique_ptr<BaseMouseBehaviorLayer>> m_MouseBehaviorLayers;
};
