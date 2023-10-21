#pragma once

#include "AppStateDataLayerCAD.h"
#include "SceneDataLayerCAD.h"
#include "ViewportDataLayerCAD.h"

struct MainDataLayerCAD
{
public:
	AppStateDataLayerCAD m_AppStateDataLayer;
	SceneDataLayerCAD m_SceneDataLayer;
	ViewportDataLayerCAD m_ViewportDataLayer;
};
