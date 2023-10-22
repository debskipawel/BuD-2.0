#include "MainGuiLayerCNC.h"

#include <Applications/CNC/GUI/PathList/PathListGuiLayerCNC.h>

MainGuiLayerCNC::MainGuiLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<PathListGuiLayerCNC>(m_MainDataLayer));
}

void MainGuiLayerCNC::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
