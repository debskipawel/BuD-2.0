#pragma once

#include "AppStateViewModel.h"
#include "CursorViewModel.h"
#include "ObjectListViewModel.h"
#include "PropertiesViewModel.h"
#include "ViewportViewModel.h"

#include <Scene/SceneCAD.h>

struct MainViewModel
{
public:
	MainViewModel(SceneCAD& scene);

	AppStateViewModel m_AppStateViewModel;
	CursorViewModel m_CursorViewModel;
	PropertiesViewModel m_PropertiesViewModel;
	ObjectListViewModel m_ObjectListViewModel;
	ViewportViewModel m_ViewportViewModel;
};
