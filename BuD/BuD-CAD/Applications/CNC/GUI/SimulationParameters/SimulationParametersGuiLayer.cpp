#include "SimulationParametersGuiLayer.h"

#include <imgui.h>

SimulationParametersGuiLayer::SimulationParametersGuiLayer(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
}

void SimulationParametersGuiLayer::DrawGui()
{
	if (ImGui::Begin("Simulation parameters ###simulation_parameters"))
	{
		DrawParameters();
		DrawStartAndSkipButtons();

		ImGui::End();
	}
}

void SimulationParametersGuiLayer::DrawParameters()
{
	ImGui::Text("Simulation speed");
	ImGui::DragFloat("###cnc_simulation_speed", &m_MainDataLayer.m_SimulationDataLayer.m_SimulationSpeed, 0.1f, 0.1f, 10.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
}

void SimulationParametersGuiLayer::DrawStartAndSkipButtons()
{
	if (!m_MainDataLayer.m_SimulationDataLayer.GetSelectedPath())
	{
		return;
	}

	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto cursorPos = ImGui::GetCursorPos();
	auto style = ImGui::GetStyle();

	auto buttonHeight = 20;
	auto buttonHeightWithSpacing = buttonHeight + style.ItemInnerSpacing.y;
	auto fullHeight = 2 * buttonHeightWithSpacing;

	if (max.y - fullHeight > cursorPos.y)
	{
		ImGui::SetCursorPos({ min.x, max.y - fullHeight });
	}

	ImGui::Separator();

	if (m_MainDataLayer.m_SimulationDataLayer.Running())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.6f, 0.0f, 0.0f, 1.0f });

		if (ImGui::Button("Pause simulation", ImVec2(max.x - min.x, 0.0f)))
		{
			m_MainDataLayer.m_SimulationDataLayer.StopSimulation();
		}

		ImGui::PopStyleColor();
	}
	else
	{
		if (ImGui::Button("Start simulation", ImVec2(max.x - min.x, 0.0f)))
		{
			m_MainDataLayer.m_SimulationDataLayer.StartSimulation();
		}
	}

	if (ImGui::Button("Skip simulation", ImVec2(max.x - min.x, 0.0f)))
	{

	}
}
