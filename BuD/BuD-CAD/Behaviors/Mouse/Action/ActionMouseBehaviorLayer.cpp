#include "ActionMouseBehaviorLayer.h"

#include <set>

ActionMouseBehaviorLayer::ActionMouseBehaviorLayer(MainDataLayer& dataLayer)
	: BaseMouseBehaviorLayer(dataLayer)
{
}

void ActionMouseBehaviorLayer::OnLeftButtonDown(int x, int y)
{
	static std::set<AppState> actionableAppStates = { AppState::MOVE, AppState::ROTATE, AppState::SCALE };

	if (
		actionableAppStates.contains(m_MainDataLayer.m_AppStateDataLayer.m_AppState) 
		&& m_MainDataLayer.m_ViewportDataLayer.IsMouseOnViewport(x, y)
	)
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
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return;
	}

	if (m_MainDataLayer.m_AppStateDataLayer.m_InAction)
	{
		HandleActionPerform(dx, dy);
	}
}

void ActionMouseBehaviorLayer::HandleActionStart()
{
	m_MainDataLayer.m_AppStateDataLayer.m_InAction = !m_MainDataLayer.m_AppStateDataLayer.m_CameraRotating;
}

void ActionMouseBehaviorLayer::HandleActionEnd()
{
	m_MainDataLayer.m_AppStateDataLayer.m_InAction = false;
}

void ActionMouseBehaviorLayer::HandleActionPerform(int dx, int dy)
{
	// TODO: add code for transforming selected
}
