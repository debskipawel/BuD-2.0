#include "MainKeyboardBehaviorLayer.h"

MainKeyboardBehaviorLayer::MainKeyboardBehaviorLayer(MainDataLayer& mainDataLayer)
    : BaseKeyboardBehaviorLayer(mainDataLayer)
{
}

auto MainKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key) -> void
{
    for (const auto& behavior : m_BehaviorLayers)
    {
        behavior->OnKeyPress(key);
    }
}

auto MainKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key) -> void
{
    for (const auto& behavior : m_BehaviorLayers)
    {
        behavior->OnKeyRelease(key);
    }
}
