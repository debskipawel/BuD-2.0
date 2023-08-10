#include "MainGuiLayer.h"

#include <imgui.h>

#include <GUI/AppState/AppStateGuiLayer.h>
#include <GUI/Console/ConsoleGuiLayer.h>
#include <GUI/ObjectList/ObjectListGuiLayer.h>
#include <GUI/ObjectList/ObjectCreationGuiLayer.h>
#include <GUI/Properties/PropertiesGuiLayer.h>
#include <GUI/Viewport/ViewportGuiLayer.h>

MainGuiLayer::MainGuiLayer(MainViewModel& viewModel)
	: m_ViewModel(viewModel)
{
	m_GuiLayers.emplace_back(std::make_unique<AppStateGuiLayer>(m_ViewModel.m_AppStateViewModel, m_ViewModel.m_ObjectListViewModel));
	m_GuiLayers.emplace_back(std::make_unique<ConsoleGuiLayer>());
	m_GuiLayers.emplace_back(std::make_unique<ObjectListGuiLayer>(m_ViewModel.m_ObjectListViewModel));
	m_GuiLayers.emplace_back(std::make_unique<ObjectCreationGuiLayer>(m_ViewModel.m_ObjectListViewModel));
	m_GuiLayers.emplace_back(std::make_unique<PropertiesGuiLayer>(m_ViewModel.m_PropertiesViewModel));
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayer>(m_ViewModel.m_ViewportViewModel));
}

void MainGuiLayer::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
