#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <DataLayers/ViewportDataLayer.h>

struct MainDataLayer
{
	MainDataLayer();

	SceneDataLayer m_SceneDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
};
