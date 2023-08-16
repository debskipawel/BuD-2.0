#pragma once

#include <AppState.h>
#include <AxisLock.h>

struct AppStateDataLayer
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