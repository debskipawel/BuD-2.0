#pragma once

#include <Applications/Switcher/GUI/BaseGuiLayerSwitcher.h>

class MainGuiLayerSwitcher : public BaseGuiLayerSwitcher
{
public:
	MainGuiLayerSwitcher(MainDataLayerSwitcher& dataLayer);

	virtual void DrawGui() override;

protected:
	std::vector<std::unique_ptr<BaseGuiLayerSwitcher>> m_GuiLayers;
};
