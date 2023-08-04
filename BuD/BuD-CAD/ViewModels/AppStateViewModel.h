#pragma once

#include <AppState.h>
#include <AxisLock.h>

struct AppStateViewModel
{
	AppState m_AppState = AppState::IDLE;
	AxisLock m_AxisLock = AxisLock::NONE;
};
