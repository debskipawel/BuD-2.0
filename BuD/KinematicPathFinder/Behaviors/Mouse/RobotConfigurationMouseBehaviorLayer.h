#pragma once

#include <Behaviors/Mouse/BaseMouseBehaviorLayer.h>

class RobotConfigurationMouseBehaviorLayer : public BaseMouseBehaviorLayer
{
public:
	RobotConfigurationMouseBehaviorLayer(MainDataLayer& mainDataLayer);

	virtual auto OnLeftButtonDown(int x, int y) -> void override;

protected:
	virtual auto EditStartRobotConfiguration(int x, int y) -> void;
	virtual auto EditEndRobotConfiguration(int x, int y) -> void;
};
