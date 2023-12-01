#pragma once

enum class AppState : unsigned int
{
	EDIT_START,
	EDIT_END,
	ADD_OBSTACLE, 
	SIMULATION_RUNNING
};

struct AppStateDataLayer
{
	AppStateDataLayer();

	AppState m_AppState;
};
