#pragma once

#include "AppStateDataLayer.h"
#include "SceneDataLayer.h"
#include "ViewportDataLayer.h"

struct MainDataLayer
{
public:
	AppStateDataLayer m_AppStateDataLayer;
	SceneDataLayer m_SceneDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
};
