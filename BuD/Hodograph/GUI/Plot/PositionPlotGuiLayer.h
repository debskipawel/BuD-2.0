#pragma once

#include <GUI/BaseGuiLayer.h>

class PositionPlotGuiLayer : public BaseGuiLayer
{
public:
	PositionPlotGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
