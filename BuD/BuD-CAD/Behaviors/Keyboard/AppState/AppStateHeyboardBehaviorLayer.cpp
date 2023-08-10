#include "AppStateHeyboardBehaviorLayer.h"

AppStateKeyboardBehaviorLayer::AppStateKeyboardBehaviorLayer(MainViewModel& viewModel)
	: BaseKeyboardBehaviorLayer(viewModel)
{
}

void AppStateKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	if (m_ViewModel.m_AppStateViewModel.m_AppState == AppState::FROZEN)
	{
		return;
	}

	HandleAppStateChange(key);
}

void AppStateKeyboardBehaviorLayer::HandleAppStateChange(BuD::KeyboardKeys key)
{
	auto result = s_AppStateMap.find(key);

	if (result != s_AppStateMap.end())
	{
		auto& scene = m_ViewModel.m_ObjectListViewModel.m_Scene;

		auto& cursor = scene.m_MainCursor;
		cursor->SetAppState(result->second);

		m_ViewModel.m_AppStateViewModel.m_AppState = result->second;
	}
}
