#include "AppStateDataLayer.h"

void AppStateDataLayer::SetAppState(AppState appState)
{
	m_AppState = appState;
}

void AppStateDataLayer::SetAxisLock(AxisLock axisLock)
{
	m_AxisLock = axisLock;
}

void AppStateDataLayer::Freeze()
{
	m_PreviousAppState = m_AppState;
	m_AppState = AppState::FROZEN;
}

void AppStateDataLayer::Unfreeze()
{
	m_AppState = m_PreviousAppState;
}
