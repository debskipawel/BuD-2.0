#include "KeyboardBehaviorLayer.h"

KeyboardBehaviorLayer::KeyboardBehaviorLayer(MainViewModel& viewModel)
	: m_ViewModel(viewModel)
{
}

void KeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
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
		m_ViewModel.m_AppStateViewModel.m_AppState = result->second;
	}
}

void KeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
}
