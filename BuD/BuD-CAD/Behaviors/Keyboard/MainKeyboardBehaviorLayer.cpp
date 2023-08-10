#include "MainKeyboardBehaviorLayer.h"

#include <Behaviors/Keyboard/AppState/AppStateHeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/AxisLock/AxisLockKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Camera/CameraKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Multiselect/MultiselectKeyboardBehaviorLayer.h>

MainKeyboardBehaviorLayer::MainKeyboardBehaviorLayer(MainViewModel& viewModel)
	: BaseKeyboardBehaviorLayer(viewModel)
{
	m_BehaviorLayers.emplace_back(std::make_unique<AppStateKeyboardBehaviorLayer>(viewModel));
	m_BehaviorLayers.emplace_back(std::make_unique<AxisLockKeyboardBehaviorLayer>(viewModel));
	m_BehaviorLayers.emplace_back(std::make_unique<CameraKeyboardBehaviorLayer>(viewModel));
	m_BehaviorLayers.emplace_back(std::make_unique<MultiselectKeyboardBehaviorLayer>(viewModel));
}

void MainKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnKeyPress(key);
	}
}

void MainKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	for (auto& layer : m_BehaviorLayers)
	{
		layer->OnKeyRelease(key);
	}
}
