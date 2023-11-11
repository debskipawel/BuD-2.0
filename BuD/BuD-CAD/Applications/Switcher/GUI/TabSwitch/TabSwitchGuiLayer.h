#pragma once

#include <Applications/Switcher/GUI/BaseGuiLayerSwitcher.h>

class TabSwitchGuiLayer : public BaseGuiLayerSwitcher
{
public:
	TabSwitchGuiLayer(MainDataLayerSwitcher& dataLayer);

	virtual void DrawGui() override;
};
