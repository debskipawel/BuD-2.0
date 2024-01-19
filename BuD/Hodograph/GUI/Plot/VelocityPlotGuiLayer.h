#pragma once

#include <GUI/BaseGuiLayer.h>

class VelocityPlotGuiLayer : public BaseGuiLayer
{
public:
	VelocityPlotGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
