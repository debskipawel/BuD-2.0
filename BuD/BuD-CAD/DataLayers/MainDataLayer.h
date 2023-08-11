#pragma once

#include "AppStateDataLayer.h"
#include "SceneDataLayer.h"
#include "ViewportDataLayer.h"

#include <Scene/SceneCAD.h>

struct MainDataLayer
{
public:
	MainDataLayer(SceneCAD& scene);

	AppStateDataLayer m_AppStateDataLayer;
	SceneDataLayer m_SceneDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
};
