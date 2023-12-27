#pragma once

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class MainMouseBehaviorLayer : public BaseMouseBehaviorLayer
{
public:
	MainMouseBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnLeftButtonDown(int x, int y) -> void override;
	virtual auto OnMiddleButtonDown(int x, int y) -> void override;
	virtual auto OnRightButtonDown(int x, int y) -> void override;

	virtual auto OnLeftButtonUp(int x, int y) -> void override;
	virtual auto OnMiddleButtonUp(int x, int y) -> void override;
	virtual auto OnRightButtonUp(int x, int y) -> void override;

	virtual auto OnMove(int x, int y, int dx, int dy) -> void override;
	virtual auto OnScroll(int x, int y, int delta) -> void override;

protected:
	std::vector<std::unique_ptr<BaseMouseBehaviorLayer>> m_BehaviorLayers;
};
