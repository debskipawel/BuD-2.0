#include "AppStateViewModel.h"

void AppStateViewModel::Freeze()
{
	m_PreviousAppState = m_AppState;
	m_AppState = AppState::FROZEN;
}

void AppStateViewModel::Unfreeze()
{
	m_AppState = m_PreviousAppState;
}
