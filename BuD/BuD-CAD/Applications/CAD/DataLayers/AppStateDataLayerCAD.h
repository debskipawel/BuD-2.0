#pragma once

#include <Applications/CAD/AppState.h>
#include <Applications/CAD/AxisLock.h>

struct AppStateDataLayerCAD
{
	AppState m_AppState = AppState::IDLE;
	AxisLock m_AxisLock = AxisLock::NONE;

	bool m_MultiselectOn = false;
	bool m_CameraReadyToMove = false;
	bool m_CameraRotating = false;
	bool m_InAction = false;

	void SetAppState(AppState appState);
	void SetAxisLock(AxisLock axisLock);

	void Freeze();
	void Unfreeze();

private:
	AppState m_PreviousAppState = AppState::IDLE;
};
