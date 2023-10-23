#pragma once

#include "AppStateDataLayerCAD.h"
#include "SceneDataLayerCAD.h"
#include <Applications/Shared/DataLayers/ViewportDataLayer.h>

struct MainDataLayerCAD
{
public:
	AppStateDataLayerCAD m_AppStateDataLayer;
	SceneDataLayerCAD m_SceneDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
};
