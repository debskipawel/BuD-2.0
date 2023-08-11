#include "AxisLockKeyboardBehaviorLayer.h"

AxisLockKeyboardBehaviorLayer::AxisLockKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayer(dataLayer)
{
}

void AxisLockKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	if (m_MainDataLayer.m_AppStateDataLayer.m_AppState == AppState::FROZEN)
	{
		return;
	}

	HandleAxisLockEnable(key);
}

void AxisLockKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	HandleAxisLockDisable(key);
}

void AxisLockKeyboardBehaviorLayer::HandleAxisLockEnable(BuD::KeyboardKeys key)
{
	auto result = s_AxisLockMap.find(key);

	if (result == s_AxisLockMap.end())
	{
		return;
	}

	auto axisLock = result->second;

	std::stack<AxisLock> tempAxisLockStack = {};
	bool foundMatchingAxisLock = false;

	while (!s_AxisLockStack.empty())
	{
		if (s_AxisLockStack.top() == axisLock)
		{
			foundMatchingAxisLock = true;
			break;
		}

		tempAxisLockStack.push(s_AxisLockStack.top());
		s_AxisLockStack.pop();
	}

	while (!tempAxisLockStack.empty())
	{
		s_AxisLockStack.push(tempAxisLockStack.top());
		tempAxisLockStack.pop();
	}

	if (foundMatchingAxisLock)
	{
		return;
	}

	s_AxisLockStack.push(axisLock);

	SetAxisLock(axisLock);
}

void AxisLockKeyboardBehaviorLayer::HandleAxisLockDisable(BuD::KeyboardKeys key)
{
	auto result = s_AxisLockMap.find(key);

	if (result == s_AxisLockMap.end())
	{
		return;
	}

	auto axisLock = result->second;

	if (!s_AxisLockStack.empty() && s_AxisLockStack.top() == axisLock)
	{
		s_AxisLockStack.pop();

		if (!s_AxisLockStack.empty())
		{
			SetAxisLock(s_AxisLockStack.top());
			return;
		}

		SetAxisLock(AxisLock::NONE);
		return;
	}

	std::stack<AxisLock> tempAxisLockStack = {};

	while (!s_AxisLockStack.empty())
	{
		if (s_AxisLockStack.top() == axisLock)
		{
			s_AxisLockStack.pop();
			break;
		}

		tempAxisLockStack.push(s_AxisLockStack.top());
		s_AxisLockStack.pop();
	}

	while (!tempAxisLockStack.empty())
	{
		s_AxisLockStack.push(tempAxisLockStack.top());
		tempAxisLockStack.pop();
	}
}

void AxisLockKeyboardBehaviorLayer::SetAxisLock(AxisLock axisLock)
{
	auto& appState = m_MainDataLayer.m_AppStateDataLayer;
	appState.SetAxisLock(axisLock);

	auto& scene = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
	auto& cursor = scene.m_MainCursor;

	cursor->SetAxisLock(axisLock);
}
