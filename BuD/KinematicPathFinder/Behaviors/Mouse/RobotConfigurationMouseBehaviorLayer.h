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

	virtual auto SolveInverseKinematic(const dxm::Vector2& j0, const dxm::Vector2& j2, float L1, float L2) -> std::vector<dxm::Vector2>;
};
