#include "PositionPlotGuiLayer.h"

#include <imgui.h>
#include <implot.h>

PositionPlotGuiLayer::PositionPlotGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto PositionPlotGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Position plot"))
	{
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();

		auto& simulation = m_MainDataLayer.m_SimulationDataLayer;

		if (simulation.IsRunning())
		{
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
		}

		if (ImPlot::BeginPlot("Position", ImVec2(max.x - min.x, max.y - min.y)))
		{
			ImPlot::SetupAxes("Time", "Position");

			ImPlot::PlotLine("Position", simulation.m_TimeValues.data(), simulation.m_PositionValues.data(), simulation.m_TimeValues.size());
			ImPlot::EndPlot();
		}

		ImGui::End();
	}
}
