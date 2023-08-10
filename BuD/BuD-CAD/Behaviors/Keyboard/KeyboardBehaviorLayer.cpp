#include "KeyboardBehaviorLayer.h"

KeyboardBehaviorLayer::KeyboardBehaviorLayer(MainViewModel& viewModel)
	: m_ViewModel(viewModel)
{
}

void KeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	auto& appStateVM = m_ViewModel.m_AppStateViewModel;

	if (appStateVM.m_AppState == AppState::FROZEN)
	{
		return;
	}

	HandleAppStateChange(key);
	HandleAxisLockEnable(key);
	HandleMultiselectEnable(key);
}

void KeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	auto& appStateVM = m_ViewModel.m_AppStateViewModel;

	HandleAxisLockDisable(key);
	HandleMultiselectDisable(key);
}

void KeyboardBehaviorLayer::HandleMultiselectEnable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Control)
	{
		m_ViewModel.m_AppStateViewModel.m_MultiselectOn = true;
	}
}

void KeyboardBehaviorLayer::HandleMultiselectDisable(BuD::KeyboardKeys key)
{
	if (key == BuD::KeyboardKeys::Control)
	{
		m_ViewModel.m_AppStateViewModel.m_MultiselectOn = false;
	}
}

void KeyboardBehaviorLayer::HandleAppStateChange(BuD::KeyboardKeys key)
{
	static std::map<BuD::KeyboardKeys, AppState> stateMap =
	{
		{ BuD::KeyboardKeys::D1, AppState::IDLE },
		{ BuD::KeyboardKeys::D2, AppState::MOVE },
		{ BuD::KeyboardKeys::D3, AppState::ROTATE },
		{ BuD::KeyboardKeys::D4, AppState::SCALE },
	};

	auto result = stateMap.find(key);

	if (result != stateMap.end())
	{
		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

		auto& cursor = scene.m_MainCursor;
		cursor->SetAppState(result->second);

		m_ViewModel.m_AppStateViewModel.m_AppState = result->second;
	}
}

static std::map<BuD::KeyboardKeys, AxisLock> axisLockMap =
{
	{ BuD::KeyboardKeys::X, AxisLock::X },
	{ BuD::KeyboardKeys::Y, AxisLock::Y },
	{ BuD::KeyboardKeys::Z, AxisLock::Z },
};

static std::stack<AxisLock> axisLockStack = {};

void KeyboardBehaviorLayer::HandleAxisLockEnable(BuD::KeyboardKeys key)
{
	auto result = axisLockMap.find(key);

	if (result == axisLockMap.end())
	{
		return;
	}

	auto axisLock = result->second;

	std::stack<AxisLock> tempAxisLockStack = {};
	bool foundMatchingAxisLock = false;

	while (!axisLockStack.empty())
	{
		if (axisLockStack.top() == axisLock)
		{
			foundMatchingAxisLock = true;
			break;
		}

		tempAxisLockStack.push(axisLockStack.top());
		axisLockStack.pop();
	}

	while (!tempAxisLockStack.empty())
	{
		axisLockStack.push(tempAxisLockStack.top());
		tempAxisLockStack.pop();
	}

	if (foundMatchingAxisLock)
	{
		return;
	}

	axisLockStack.push(axisLock);

	SetAxisLock(axisLock);
}

void KeyboardBehaviorLayer::HandleAxisLockDisable(BuD::KeyboardKeys key)
{
	auto result = axisLockMap.find(key);

	if (result == axisLockMap.end())
	{
		return;
	}

	auto axisLock = result->second;

	if (!axisLockStack.empty() && axisLockStack.top() == axisLock)
	{
		axisLockStack.pop();

		if (!axisLockStack.empty())
		{
			SetAxisLock(axisLockStack.top());
			return;
		}

		SetAxisLock(AxisLock::NONE);
		return;
	}

	std::stack<AxisLock> tempAxisLockStack = {};

	while (!axisLockStack.empty())
	{
		if (axisLockStack.top() == axisLock)
		{
			axisLockStack.pop();
			break;
		}

		tempAxisLockStack.push(axisLockStack.top());
		axisLockStack.pop();
	}

	while (!tempAxisLockStack.empty())
	{
		axisLockStack.push(tempAxisLockStack.top());
		tempAxisLockStack.pop();
	}
}

void KeyboardBehaviorLayer::SetAxisLock(AxisLock axisLock)
{
	auto& appState = m_ViewModel.m_AppStateViewModel;
	appState.m_AxisLock = axisLock;

	auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;
	auto& cursor = scene.m_MainCursor;

	cursor->SetAxisLock(axisLock);
}
