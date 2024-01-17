#include "TestPlotGuiLayer.h"

#include <implot.h>

TestPlotGuiLayer::TestPlotGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
	ImPlot::CreateContext();
}

TestPlotGuiLayer::~TestPlotGuiLayer()
{
	ImPlot::DestroyContext();
}

auto TestPlotGuiLayer::DrawGui() -> void
{
	ImGui::Begin("My Window");
	
	auto barData = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

	if (ImPlot::BeginPlot("My Plot")) 
	{
		ImPlot::PlotBars("Test Plot", barData.data(), barData.size());
		
		ImPlot::EndPlot();
	}
	
	ImGui::End();
}
