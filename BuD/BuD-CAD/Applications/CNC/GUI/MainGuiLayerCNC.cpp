#include "MainGuiLayerCNC.h"

#include <Applications/CNC/GUI/MenuBar/MenuBarGuiLayerCNC.h>
#include <Applications/CNC/GUI/SimulationParameters/SimulationParametersGuiLayer.h>
#include <Applications/CNC/GUI/StartingParameters/StartingParametersGuiLayer.h>
#include <Applications/CNC/GUI/PathList/PathListGuiLayerCNC.h>
#include <Applications/CNC/GUI/Viewport/ViewportGuiLayerCNC.h>

MainGuiLayerCNC::MainGuiLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<MenuBarGuiLayerCNC>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<SimulationParametersGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<StartingParametersGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<PathListGuiLayerCNC>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayerCNC>(m_MainDataLayer));
}

void MainGuiLayerCNC::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
