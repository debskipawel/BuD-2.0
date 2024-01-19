#pragma once

#include <GUI/BaseGuiLayer.h>

class AccelerationPlotGuiLayer : public BaseGuiLayer
{
public:
	AccelerationPlotGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
