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
}

void SimulationParametersGuiLayer::DrawStartAndSkipButtons()
{
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

	if (ImGui::Button("Start simulation", ImVec2(max.x - min.x, 0.0f)))
	{

	}

	if (ImGui::Button("Skip simulation", ImVec2(max.x - min.x, 0.0f)))
	{

	}
}
