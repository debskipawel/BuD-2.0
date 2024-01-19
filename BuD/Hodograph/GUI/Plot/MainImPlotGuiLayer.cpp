#include "MainImPlotGuiLayer.h"

#include <implot.h>

#include <GUI/Plot/AccelerationPlotGuiLayer.h>
#include <GUI/Plot/PositionPlotGuiLayer.h>
#include <GUI/Plot/PhasePlotGuiLayer.h>
#include <GUI/Plot/VelocityPlotGuiLayer.h>

MainImPlotGuiLayer::MainImPlotGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
	m_ImPlotGuiLayers.emplace_back(std::make_unique<AccelerationPlotGuiLayer>(m_MainDataLayer));
	m_ImPlotGuiLayers.emplace_back(std::make_unique<PhasePlotGuiLayer>(m_MainDataLayer));
	m_ImPlotGuiLayers.emplace_back(std::make_unique<PositionPlotGuiLayer>(m_MainDataLayer));
	m_ImPlotGuiLayers.emplace_back(std::make_unique<VelocityPlotGuiLayer>(m_MainDataLayer));

	ImPlot::CreateContext();
}

MainImPlotGuiLayer::~MainImPlotGuiLayer()
{
	ImPlot::DestroyContext();
}

auto MainImPlotGuiLayer::DrawGui() -> void
{
	for (const auto& implotGuiLayer : m_ImPlotGuiLayers)
	{
		implotGuiLayer->DrawGui();
	}
}
