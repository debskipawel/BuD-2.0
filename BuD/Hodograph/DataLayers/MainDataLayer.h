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
};
