#include "PhasePlotGuiLayer.h"

#include <imgui.h>
#include <implot.h>

PhasePlotGuiLayer::PhasePlotGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto PhasePlotGuiLayer::DrawGui() -> void
{
	if (!m_MainDataLayer.m_DisplayPhasePlot)
	{
		return;
	}

	if (ImGui::Begin("Phase plot"))
	{
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();

		auto& simulation = m_MainDataLayer.m_SimulationDataLayer;

		if (simulation.IsRunning())
		{
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
		}

		if (ImPlot::BeginPlot("Phase", ImVec2(max.x - min.x, max.y - min.y)))
		{
			ImPlot::SetupAxes("Position", "Velocity");

			ImPlot::PlotLine("Phase", simulation.m_PositionValues.data(), simulation.m_VelocityValues.data(), simulation.m_PositionValues.size());
			ImPlot::EndPlot();
		}

		ImGui::End();
	}
}
