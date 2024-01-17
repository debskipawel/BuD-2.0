#pragma once

#include <GUI/BaseGuiLayer.h>

class TestPlotGuiLayer : public BaseGuiLayer
{
public:
	TestPlotGuiLayer(MainDataLayer& mainDataLayer);
	virtual ~TestPlotGuiLayer();

	virtual auto DrawGui() -> void override;
};
