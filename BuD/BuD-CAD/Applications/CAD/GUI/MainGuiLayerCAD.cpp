#include "MainGuiLayerCAD.h"

#include <imgui.h>

#include <Applications/CAD/GUI/AppState/AppStateGuiLayer.h>
#include <Applications/CAD/GUI/Cursor/CursorGuiLayer.h>
#include <Applications/CAD/GUI/Console/ConsoleGuiLayer.h>
#include <Applications/CAD/GUI/Intersection/IntersectionGuiLayer.h>
#include <Applications/CAD/GUI/MenuBar/MenuBarGuiLayer.h>
#include <Applications/CAD/GUI/ObjectList/ObjectListGuiLayer.h>
#include <Applications/CAD/GUI/ObjectList/ObjectCreationGuiLayer.h>
#include <Applications/CAD/GUI/Performance/PerformanceGuiLayer.h>
#include <Applications/CAD/GUI/Properties/PropertiesGuiLayer.h>
#include <Applications/CAD/GUI/Viewport/ViewportGuiLayer.h>

MainGuiLayerCAD::MainGuiLayerCAD(MainDataLayerCAD& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<AppStateGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<CursorGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ConsoleGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<IntersectionGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<MenuBarGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ObjectListGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ObjectCreationGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<PerformanceGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<PropertiesGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayer>(m_MainDataLayer));
}

void MainGuiLayerCAD::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
