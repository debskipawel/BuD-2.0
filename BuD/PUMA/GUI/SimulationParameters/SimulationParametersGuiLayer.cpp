#include "SimulationParametersGuiLayer.h"

#include <imgui.h>

SimulationParametersGuiLayer::SimulationParametersGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{

}

auto SimulationParametersGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Simulation parameters"))
	{
		ImGui::Text("Simulation speed");

		ImGui::DragFloat("###simulation_speed", &m_MainDataLayer.m_SimulationSpeed, 0.1f, 0.1f, 10.0f, "%.1fx", ImGuiSliderFlags_AlwaysClamp);

		ImGui::End();
	}
}
