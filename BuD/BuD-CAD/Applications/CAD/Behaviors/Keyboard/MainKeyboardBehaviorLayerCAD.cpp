#include "MainKeyboardBehaviorLayerCAD.h"

#include <Applications/CAD/Behaviors/Keyboard/AppState/AppStateHeyboardBehaviorLayer.h>
#include <Applications/CAD/Behaviors/Keyboard/AxisLock/AxisLockKeyboardBehaviorLayer.h>
#include <Applications/CAD/Behaviors/Keyboard/Camera/CameraKeyboardBehaviorLayer.h>
#include <Applications/CAD/Behaviors/Keyboard/Shortcuts/ShortcutsKeyboardBehaviorLayer.h>
#include <Applications/CAD/Behaviors/Keyboard/Multiselect/MultiselectKeyboardBehaviorLayer.h>

MainKeyboardBehaviorLayerCAD::MainKeyboardBehaviorLayerCAD(MainDataLayerCAD& dataLayer)
	: BaseKeyboardBehaviorLayerCAD(dataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<ShortcutsKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<AppStateKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<AxisLockKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<CameraKeyboardBehaviorLayer>(dataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<MultiselectKeyboardBehaviorLayer>(dataLayer));
}

bool MainKeyboardBehaviorLayerCAD::OnKeyPress(BuD::KeyboardKeys key)
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

bool MainKeyboardBehaviorLayerCAD::OnKeyRelease(BuD::KeyboardKeys key)
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
