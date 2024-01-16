#include "MainGuiLayer.h"

#include <imgui.h>

#include <GUI/KeyFrameList/KeyFrameListGuiLayer.h>
#include <GUI/Timeline/TimelineGuiLayer.h>
#include <GUI/Viewport/ViewportGuiLayer.h>
#include <GUI/RobotParameters/RobotParametersGuiLayer.h>
#include <GUI/SimulationParameters/SimulationParametersGuiLayer.h>

MainGuiLayer::MainGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
	m_GuiLayers.emplace_back(std::make_unique<KeyFrameListGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<RobotParametersGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<SimulationParametersGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<TimelineGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayer>(m_MainDataLayer));
}

void MainGuiLayer::DrawGui()
{
	for (auto& layer : m_GuiLayers)
	{
		layer->DrawGui();
	}
}
