#include "MainKeyboardBehaviorLayer.h"

#include <Behaviors/Keyboard/AppState/AppStateHeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/AxisLock/AxisLockKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Camera/CameraKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Shortcuts/ShortcutsKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Multiselect/MultiselectKeyboardBehaviorLayer.h>

MainKeyboardBehaviorLayer::MainKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayer(dataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<ShortcutsKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<AppStateKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<AxisLockKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<CameraKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<MultiselectKeyboardBehaviorLayer>(dataLayer));
}

bool MainKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	for (auto& layer : m_BehaviorLayers)
	{
		auto handled = layer->OnKeyPress(key);

		if (handled)
		{
			break;
		}
	}

	return true;
}

bool MainKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	for (auto& layer : m_BehaviorLayers)
	{
		auto handled = layer->OnKeyRelease(key);

		if (handled)
		{
			break;
		}
	}

	return true;
}
