#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

class AppStateKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	AppStateKeyboardBehaviorLayer(MainViewModel& viewModel);

	virtual void OnKeyPress(BuD::KeyboardKeys key) override;

protected:
	virtual void HandleAppStateChange(BuD::KeyboardKeys key);

	inline static std::map<BuD::KeyboardKeys, AppState> s_AppStateMap =
	{
		{ BuD::KeyboardKeys::D1, AppState::IDLE },
		{ BuD::KeyboardKeys::D2, AppState::MOVE },
		{ BuD::KeyboardKeys::D3, AppState::ROTATE },
		{ BuD::KeyboardKeys::D4, AppState::SCALE },
	};
};
