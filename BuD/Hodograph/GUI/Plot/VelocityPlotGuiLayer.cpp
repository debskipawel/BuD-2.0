#include "VelocityPlotGuiLayer.h"

#include <imgui.h>
#include <implot.h>

VelocityPlotGuiLayer::VelocityPlotGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
}

auto VelocityPlotGuiLayer::DrawGui() -> void
{
	if (!m_MainDataLayer.m_DisplayVelocityPlot)
	{
		return;
	}

	if (ImGui::Begin("Velocity plot"))
	{
		auto min = ImGui::GetWindowContentRegionMin();
		auto max = ImGui::GetWindowContentRegionMax();

		auto& simulation = m_MainDataLayer.m_SimulationDataLayer;

		if (simulation.IsRunning())
		{
			ImPlot::SetNextAxisToFit(ImAxis_X1);
			ImPlot::SetNextAxisToFit(ImAxis_Y1);
		}

		if (ImPlot::BeginPlot("Velocity", ImVec2(max.x - min.x, max.y - min.y)))
		{
			ImPlot::SetupAxes("Time", "Velocity");

			ImPlot::PlotLine("Velocity", simulation.m_TimeValues.data(), simulation.m_VelocityValues.data(), simulation.m_TimeValues.size());
			ImPlot::EndPlot();
		}

		ImGui::End();
	}
}
