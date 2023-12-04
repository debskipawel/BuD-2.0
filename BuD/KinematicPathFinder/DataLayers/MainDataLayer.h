#pragma once

#include <DataLayers/AppStateDataLayer.h>
#include <DataLayers/Scene/SceneDataLayer.h>
#include <DataLayers/ViewportDataLayer.h>

struct MainDataLayer
{
	MainDataLayer();

	AppStateDataLayer m_AppStateDataLayer;
	SceneDataLayer m_SceneDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
};
