#include "CameraKeyboardBehaviorLayer.h"

CameraKeyboardBehaviorLayer::CameraKeyboardBehaviorLayer(MainDataLayer& mainDataLayer)
	: BaseKeyboardBehaviorLayer(mainDataLayer)
{
}

auto CameraKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key) -> void
{
	if (key == BuD::KeyboardKeys::Q)
	{
		m_MainDataLayer.m_CameraDataLayer.m_RotateCamera = true;
	}
}

auto CameraKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key) -> void
{
	if (key == BuD::KeyboardKeys::Q)
	{
		m_MainDataLayer.m_CameraDataLayer.m_RotateCamera = false;
	}
}
