#pragma once

#include <DataLayers/Scene/SceneDataLayer.h>
#include <DataLayers/Viewport/ViewportDataLayer.h>
#include <DataLayers/Simulation/SimulationDataLayer.h>

struct MainDataLayer
{
	MainDataLayer();

	SceneDataLayer m_SceneDataLayer;
	SimulationDataLayer m_SimulationDataLayer;
	ViewportDataLayer m_ViewportDataLayer;

	bool m_DisplayPositionPlot;
	bool m_DisplayVelocityPlot;
	bool m_DisplayAccelerationPlot;
	bool m_DisplayPhasePlot;
};
