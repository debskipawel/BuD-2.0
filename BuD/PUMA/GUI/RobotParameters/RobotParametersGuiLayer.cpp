#include "RobotParametersGuiLayer.h"

#include <imgui.h>

RobotParametersGuiLayer::RobotParametersGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto RobotParametersGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Robot parameters"))
	{
		auto& robotParameters = m_MainDataLayer.m_RobotParameters;

		auto modified = false;

		ImGui::Text("Arm length 1");
		
		modified |= ImGui::DragFloat("###arm_length_1", &robotParameters.m_L1, 0.1f, 0.0f, 10.0f, "%.1f [cm]", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Arm length 3");

		modified |= ImGui::DragFloat("###arm_length_3", &robotParameters.m_L3, 0.1f, 0.0f, 10.0f, "%.1f [cm]", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Arm length 4");

		modified |= ImGui::DragFloat("###arm_length_4", &robotParameters.m_L4, 0.1f, 0.0f, 10.0f, "%.1f [cm]", ImGuiSliderFlags_AlwaysClamp);

		if (modified)
		{
			m_MainDataLayer.RevalidateSimulationMeshes();
		}

		ImGui::End();
	}
}
