#include "MainGuiLayer.h"

#include <imgui.h>

#include <GUI/Viewport/ViewportGuiLayer.h>

MainGuiLayer::MainGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayer>(m_MainDataLayer));
}

void MainGuiLayer::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
