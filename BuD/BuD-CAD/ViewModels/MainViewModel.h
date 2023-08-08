#pragma once

#include "AppStateViewModel.h"
#include "ObjectListViewModel.h"
#include "PropertiesViewModel.h"
#include "ViewportViewModel.h"

#include <Scene/SceneCAD.h>

struct MainViewModel
{
public:
	MainViewModel(SceneCAD& scene);

	AppStateViewModel m_AppStateViewModel;
	PropertiesViewModel m_PropertiesViewModel;
	ObjectListViewModel m_ObjectListViewModel;
	ViewportViewModel m_ViewportViewModel;
};
