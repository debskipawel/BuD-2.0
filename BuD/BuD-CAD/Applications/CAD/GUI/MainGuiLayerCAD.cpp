#include "MainGuiLayerCAD.h"

#include <imgui.h>

#include <Applications/CAD/GUI/AppState/AppStateGuiLayer.h>
#include <Applications/CAD/GUI/Cursor/CursorGuiLayer.h>
#include <Applications/CAD/GUI/Intersection/IntersectionGuiLayer.h>
#include <Applications/CAD/GUI/MenuBar/MenuBarGuiLayerCAD.h>
#include <Applications/CAD/GUI/ObjectList/ObjectListGuiLayer.h>
#include <Applications/CAD/GUI/ObjectList/ObjectCreationGuiLayer.h>
#include <Applications/CAD/GUI/Properties/PropertiesGuiLayer.h>
#include <Applications/CAD/GUI/Viewport/ViewportGuiLayerCAD.h>

#include <Applications/Shared/GUI/Console/ConsoleGuiLayer.h>
#include <Applications/Shared/GUI/Performance/PerformanceGuiLayer.h>

MainGuiLayerCAD::MainGuiLayerCAD(MainDataLayerCAD& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<AppStateGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<CursorGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ConsoleGuiLayer>("cad_console"));
	m_GuiLayers.emplace_back(std::make_unique<IntersectionGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<MenuBarGuiLayerCAD>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ObjectListGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ObjectCreationGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<PerformanceGuiLayer>("cad_performance"));
	m_GuiLayers.emplace_back(std::make_unique<PropertiesGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayerCAD>(m_MainDataLayer));
}

void MainGuiLayerCAD::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
