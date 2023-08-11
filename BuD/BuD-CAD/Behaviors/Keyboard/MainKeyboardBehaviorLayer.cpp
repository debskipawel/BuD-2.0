#include "MainKeyboardBehaviorLayer.h"

#include <Behaviors/Keyboard/AppState/AppStateHeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/AxisLock/AxisLockKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Camera/CameraKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Multiselect/MultiselectKeyboardBehaviorLayer.h>

MainKeyboardBehaviorLayer::MainKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayer(dataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<AppStateKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<AxisLockKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<CameraKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<MultiselectKeyboardBehaviorLayer>(dataLayer));
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
