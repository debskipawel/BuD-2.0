#include "AccelerationPlotGuiLayer.h"

#include <imgui.h>
#include <implot.h>

AccelerationPlotGuiLayer::AccelerationPlotGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto AccelerationPlotGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Acceleration plot"))
	{
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();

		auto& simulation = m_MainDataLayer.m_SimulationDataLayer;

		if (simulation.IsRunning())
		{
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
		}

		if (ImPlot::BeginPlot("Acceleration", ImVec2(max.x - min.x, max.y - min.y)))
		{
			ImPlot::SetupAxes("Time", "Acceleration");

			ImPlot::PlotLine("Acceleration", simulation.m_TimeValues.data(), simulation.m_AccelerationValues.data(), simulation.m_TimeValues.size());
			ImPlot::EndPlot();
		}

		ImGui::End();
	}
}
