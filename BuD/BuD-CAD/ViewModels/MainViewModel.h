#pragma once

#include "AppStateViewModel.h"
#include "ObjectListViewModel.h"
#include "ViewportViewModel.h"

#include <AppState.h>

struct MainViewModel
{
	AppStateViewModel m_AppStateViewModel;
	ObjectListViewModel m_ObjectListViewModel;
	ViewportViewModel m_ViewportViewModel;
};
