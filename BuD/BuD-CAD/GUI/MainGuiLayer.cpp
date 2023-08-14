#include "MainGuiLayer.h"

#include <imgui.h>

#include <GUI/AppState/AppStateGuiLayer.h>
#include <GUI/Cursor/CursorGuiLayer.h>
#include <GUI/Console/ConsoleGuiLayer.h>
#include <GUI/MenuBar/MenuBarGuiLayer.h>
#include <GUI/ObjectList/ObjectListGuiLayer.h>
#include <GUI/ObjectList/ObjectCreationGuiLayer.h>
#include <GUI/Performance/PerformanceGuiLayer.h>
#include <GUI/Properties/PropertiesGuiLayer.h>
#include <GUI/Viewport/ViewportGuiLayer.h>

MainGuiLayer::MainGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<AppStateGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<CursorGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ConsoleGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<MenuBarGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ObjectListGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ObjectCreationGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<PerformanceGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<PropertiesGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayer>(m_MainDataLayer));
}

void MainGuiLayer::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
