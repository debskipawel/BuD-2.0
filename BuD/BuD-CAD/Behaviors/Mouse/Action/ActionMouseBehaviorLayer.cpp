#include "ActionMouseBehaviorLayer.h"

#include <set>

ActionMouseBehaviorLayer::ActionMouseBehaviorLayer(MainViewModel& viewModel)
	: BaseMouseBehaviorLayer(viewModel)
{
}

void ActionMouseBehaviorLayer::OnLeftButtonDown(int x, int y)
{
	static std::set<AppState> actionableAppStates = { AppState::MOVE, AppState::ROTATE, AppState::SCALE };

	if (actionableAppStates.contains(m_ViewModel.m_AppStateViewModel.m_AppState) && IsMouseOnViewport(x, y))
	{
		HandleActionStart();
	}
}

void ActionMouseBehaviorLayer::OnLeftButtonUp(int x, int y)
{
	HandleActionEnd();
}

void ActionMouseBehaviorLayer::OnMouseMove(int dx, int dy)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_ViewModel.m_AppStateViewModel.m_InAction)
	{
		HandleActionPerform(dx, dy);
	}
}

void ActionMouseBehaviorLayer::HandleActionStart()
{
	m_ViewModel.m_AppStateViewModel.m_InAction = !m_ViewModel.m_AppStateViewModel.m_CameraRotating;
}

void ActionMouseBehaviorLayer::HandleActionEnd()
{
	m_ViewModel.m_AppStateViewModel.m_InAction = false;
}

void ActionMouseBehaviorLayer::HandleActionPerform(int dx, int dy)
{
	// TODO: add code for transforming selected
}
