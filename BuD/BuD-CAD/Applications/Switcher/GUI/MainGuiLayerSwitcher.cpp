#include "MainGuiLayerSwitcher.h"

#include <Applications/Switcher/GUI/TabSwitch/TabSwitchGuiLayer.h>

MainGuiLayerSwitcher::MainGuiLayerSwitcher(MainDataLayerSwitcher& dataLayer)
	: BaseGuiLayerSwitcher(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<TabSwitchGuiLayer>(m_MainDataLayer));
}

void MainGuiLayerSwitcher::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
