#pragma once

#include <Applications/CNC/DataLayers/PathListDataLayer.h>
#include <Applications/CNC/DataLayers/SceneDataLayerCNC.h>

#include <Applications/Shared/DataLayers/ViewportDataLayer.h>

struct MainDataLayerCNC
{
	PathListDataLayer m_PathListDataLayer;
	SceneDataLayerCNC m_SceneDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
};
