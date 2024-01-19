#pragma once

#include <GUI/BaseGuiLayer.h>

class MainImPlotGuiLayer : public BaseGuiLayer
{
public:
	MainImPlotGuiLayer(MainDataLayer& mainDataLayer);
	virtual ~MainImPlotGuiLayer();

	virtual auto DrawGui() -> void override;

protected:
	std::vector<std::unique_ptr<BaseGuiLayer>> m_ImPlotGuiLayers;
};
