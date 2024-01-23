#include "SimulationParametersGuiLayer.h"

#include <imgui.h>

SimulationParametersGuiLayer::SimulationParametersGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto SimulationParametersGuiLayer::DrawGui() -> void
{
	ImGui::Begin("Parameters");

	auto& simulationDataLayer = m_MainDataLayer.m_SimulationDataLayer;

	auto epsilon = 0.01f;

	ImGui::Text("Arm length (L)");
	ImGui::DragFloat("###arm_length", &simulationDataLayer.m_ArmLength, epsilon, simulationDataLayer.m_Radius + epsilon, 10.0f, "%.2f [m]", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Text("Radius (R)");
	ImGui::DragFloat("###radius", &simulationDataLayer.m_Radius, epsilon, 0.1f, simulationDataLayer.m_ArmLength - epsilon, "%.2f [m]", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Text("Angular velocity");
	ImGui::DragFloat("###angular_velocity", &simulationDataLayer.m_AngularVelocity, 0.1f, 0.0f, 720.0f, "%.2f [deg/s]", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Text("Standard deviation");
	ImGui::DragFloat("###standard_deviation", &simulationDataLayer.m_StandardDeviation, 0.0001f, 0.0f, 0.01f, "%.4f", ImGuiSliderFlags_AlwaysClamp);

	ImGui::Separator();

	DrawSimulationButtons();

	ImGui::End();
}

auto SimulationParametersGuiLayer::DrawSimulationButtons() -> void
{
	auto min = ImGui::GetWindowContentRegionMin();
	auto max = ImGui::GetWindowContentRegionMax();

	auto& style = ImGui::GetStyle();

	auto width = max.x - min.x;
	auto buttonSize = ImVec2(width, 20.0f);

	auto newCursorY = max.y - buttonSize.y - style.ItemInnerSpacing.y;

	if (newCursorY >= ImGui::GetCursorPosY())
	{
		ImGui::SetCursorPosY(newCursorY);
	}

	ImGui::Separator();

	auto running = m_MainDataLayer.m_SimulationDataLayer.IsRunning();

	if (running)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));

		if (ImGui::Button("Stop simulation", buttonSize))
		{
			m_MainDataLayer.m_SimulationDataLayer.Stop();
		}

		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button("Start simulation", buttonSize))
		{
			m_MainDataLayer.m_SimulationDataLayer.Run();
		}
	}
}
