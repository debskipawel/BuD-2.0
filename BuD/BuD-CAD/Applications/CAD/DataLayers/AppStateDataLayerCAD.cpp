#include "AppStateDataLayerCAD.h"

void AppStateDataLayerCAD::SetAppState(AppState appState)
{
	m_AppState = appState;
}

void AppStateDataLayerCAD::SetAxisLock(AxisLock axisLock)
{
	m_AxisLock = axisLock;
}

void AppStateDataLayerCAD::Freeze()
{
	if (m_AppState == AppState::FROZEN)
	{
		return;
	}

	m_PreviousAppState = m_AppState;
	m_AppState = AppState::FROZEN;
}

void AppStateDataLayerCAD::Unfreeze()
{
	m_AppState = m_PreviousAppState;
}
