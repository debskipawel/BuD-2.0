#pragma once

#include <GUI/BaseGuiLayer.h>

class RobotParametersGuiLayer : public BaseGuiLayer
{
public:
	RobotParametersGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
