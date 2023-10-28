#pragma once

#include <Applications/CNC/DataLayers/PathList/PathListDataLayer.h>
#include <Applications/CNC/DataLayers/SceneDataLayerCNC.h>
#include <Applications/CNC/DataLayers/CameraDataLayerCNC.h>
#include <Applications/CNC/DataLayers/Simulation/SimulationDataLayerCNC.h>

#include <Applications/Shared/DataLayers/ViewportDataLayer.h>

struct MainDataLayerCNC
{
	SimulationDataLayerCNC m_SimulationDataLayer;
	CameraDataLayerCNC m_CameraDataLayer;

	PathListDataLayer m_PathListDataLayer;
	
	SceneDataLayerCNC m_SceneDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
};
