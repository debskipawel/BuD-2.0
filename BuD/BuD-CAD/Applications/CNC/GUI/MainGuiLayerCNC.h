#pragma once

#include <memory>

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>

class MainGuiLayerCNC : public BaseGuiLayerCNC
{
public:
	MainGuiLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;

protected:
	std::vector<std::unique_ptr<BaseGuiLayer>> m_GuiLayers;
};
