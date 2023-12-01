#include "MainGuiLayer.h"

#include <imgui.h>

#include <GUI/AppStateGuiLayer.h>
#include <GUI/RobotParametersGuiLayer.h>
#include <GUI/ViewportGuiLayer.h>

MainGuiLayer::MainGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer), m_GuiLayers()
{
	m_GuiLayers.emplace_back(std::make_unique<AppStateGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<RobotParametersGuiLayer>(m_MainDataLayer));
	m_GuiLayers.emplace_back(std::make_unique<ViewportGuiLayer>("Viewport", m_MainDataLayer));
}

auto MainGuiLayer::DrawGui() -> void
{
	auto windowPos = ImGui::GetWindowPos();

	m_MainDataLayer.m_ViewportDataLayer.m_WindowPos = { windowPos.x - 60.0f, windowPos.y - 60.0f };

	for (auto& guiLayer : m_GuiLayers)
	{
		guiLayer->DrawGui();
	}
}
