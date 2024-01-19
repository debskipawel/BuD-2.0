#pragma once

#include <GUI/BaseGuiLayer.h>

class PlotDisplayParametersGuiLayer : public BaseGuiLayer
{
public:
	PlotDisplayParametersGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
