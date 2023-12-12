#pragma once

#include <GUI/BaseGuiLayer.h>

class MainGuiLayer : public BaseGuiLayer
{
public:
	MainGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	std::vector<std::unique_ptr<BaseGuiLayer>> m_GuiLayers;
};
