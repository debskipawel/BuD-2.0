#include "MainKeyboardBehaviorLayer.h"

#include <Behaviors/Keyboard/Camera/CameraKeyboardBehaviorLayer.h>
#include <Behaviors/Keyboard/Pause/PauseKeyboardBehaviorLayer.h>

MainKeyboardBehaviorLayer::MainKeyboardBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseKeyboardBehaviorLayer(mainDataLayer)
{
	m_BehaviorLayers.emplace_back(std::make_unique<CameraKeyboardBehaviorLayer>(m_MainDataLayer));
	m_BehaviorLayers.emplace_back(std::make_unique<PauseKeyboardBehaviorLayer>(m_MainDataLayer));
}

auto MainKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnKeyPress(key);
	}
}

auto MainKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key) -> void
{
	for (const auto& behaviorLayer : m_BehaviorLayers)
	{
		behaviorLayer->OnKeyRelease(key);
	}
}
