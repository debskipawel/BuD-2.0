#include "SimulationParametersGuiLayer.h"

#include <imgui.h>

SimulationParametersGuiLayer::SimulationParametersGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto SimulationParametersGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Simulation ###simulation_params"))
	{
		auto& simulationDataLayer = m_MainDataLayer.m_SimulationDataLayer;

		ImGui::Text("Angular velocity");
		ImGui::DragFloat("###simulationSpeed", &simulationDataLayer.m_AngularVelocity, 0.1f, 10.0f, 90.0f, "%.1f deg/s", ImGuiSliderFlags_AlwaysClamp);

		ImGui::End();
	}
}
