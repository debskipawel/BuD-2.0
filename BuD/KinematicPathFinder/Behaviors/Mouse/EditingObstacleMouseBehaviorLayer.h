#pragma once

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class EditingObstacleMouseBehaviorLayer : public BaseMouseBehaviorLayer
{
public:
	EditingObstacleMouseBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnLeftButtonDown(int x, int y) -> void override;
	virtual auto OnLeftButtonUp(int x, int y) -> void override;

	virtual auto OnMouseMove(int x, int y) -> void override;

protected:
	virtual auto HandleAddingObstacle(int x, int y) -> void;

	std::shared_ptr<Obstacle> m_EditedObstacle;
};
