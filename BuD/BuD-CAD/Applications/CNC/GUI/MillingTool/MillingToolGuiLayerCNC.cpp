#include "MillingToolGuiLayerCNC.h"

#include <imgui.h>

MillingToolGuiLayerCNC::MillingToolGuiLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
}

void MillingToolGuiLayerCNC::DrawGui()
{
	if (m_MainDataLayer.m_SimulationDataLayer.Running())
	{
		return;
	}

	auto program = m_MainDataLayer.m_SimulationDataLayer.GetSelectedPath();

	if (!program)
	{
		return;
	}

	if (ImGui::Begin("Milling tool ###milling_tool"))
	{
		auto& tool = program->m_Tool;
		auto& toolParameters = tool->m_Parameters;

		ImGui::Text("Tool radius");
		ImGui::DragFloat("###milling_tool_radius", &toolParameters.m_Radius, 0.01f, 0.1f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Separator();

		ImGui::Text("Milling height");
		ImGui::DragFloat("###milling_tool_height", &toolParameters.m_Height, 0.01f, 0.1f, 5.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::End();
	}
}
