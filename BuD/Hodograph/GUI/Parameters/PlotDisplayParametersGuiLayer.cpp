#include "PlotDisplayParametersGuiLayer.h"

#include <imgui.h>

PlotDisplayParametersGuiLayer::PlotDisplayParametersGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto PlotDisplayParametersGuiLayer::DrawGui() -> void
{
	ImGui::Begin("Plot display parameters");

	ImGui::Text("Display position plot:");
	ImGui::SameLine();
	ImGui::Checkbox("###display_position_plot", &m_MainDataLayer.m_DisplayPositionPlot);

	ImGui::Text("Display velocity plot:");
	ImGui::SameLine();
	ImGui::Checkbox("###display_velocity_plot", &m_MainDataLayer.m_DisplayVelocityPlot);

	ImGui::Text("Display acceleration plot:");
	ImGui::SameLine();
	ImGui::Checkbox("###display_acceleration_plot", &m_MainDataLayer.m_DisplayAccelerationPlot);

	ImGui::Text("Display phase plot:");
	ImGui::SameLine();
	ImGui::Checkbox("###display_phase_plot", &m_MainDataLayer.m_DisplayPhasePlot);

	ImGui::End();
}
