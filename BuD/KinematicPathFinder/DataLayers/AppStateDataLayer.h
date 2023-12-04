#pragma once

enum class AppState : unsigned int
{
	EDIT_START,
	EDIT_END,
	ADD_OBSTACLE, 
	ANIMATION_RUNNING
};

struct AppStateDataLayer
{
	AppStateDataLayer();

	AppState m_AppState;
};
