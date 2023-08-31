#include "ShortcutsKeyboardBehaviorLayer.h"

#include <Visitors/Transform/UpdateTransformVisitor.h>

ShortcutsKeyboardBehaviorLayer::ShortcutsKeyboardBehaviorLayer(MainDataLayer& dataLayer)
	: BaseKeyboardBehaviorLayer(dataLayer)
{
	m_ErrorSoundEffect = BuD::Audio::AudioSystem::Load("./Resources/Sounds/powiadomienie.mp3");

	auto defaultAudioDevice = BuD::Audio::AudioSystem::DefaultAudioDevice();
	BuD::Audio::AudioSystem::SetActiveDevice(defaultAudioDevice);

	m_SystemKeysPressed =
	{
		{ BuD::KeyboardKeys::Control, false },
	};
}

bool ShortcutsKeyboardBehaviorLayer::OnKeyPress(BuD::KeyboardKeys key)
{
	if (m_SystemKeysPressed.contains(key))
	{
		m_SystemKeysPressed[key] = true;
	}

	if (m_SystemKeysPressed[BuD::KeyboardKeys::Control] && key == BuD::KeyboardKeys::Z)
	{
		Undo();

		return true;
	}

	if (m_SystemKeysPressed[BuD::KeyboardKeys::Control] && key == BuD::KeyboardKeys::Y)
	{
		Redo();

		return true;
	}

	return false;
}

bool ShortcutsKeyboardBehaviorLayer::OnKeyRelease(BuD::KeyboardKeys key)
{
	if (m_SystemKeysPressed.contains(key))
	{
		m_SystemKeysPressed[key] = false;
	}

	return false;
}

void ShortcutsKeyboardBehaviorLayer::Undo()
{
	auto undone = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Undo();

	if (undone)
	{
		auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

		cursor->SetPosition(m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Centroid());
	}
	else
	{
		BuD::Audio::AudioSystem::Play(m_ErrorSoundEffect);
	}
}

void ShortcutsKeyboardBehaviorLayer::Redo()
{
	auto redone = m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Redo();

	if (redone)
	{
		auto& cursor = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_CentroidCursor;

		cursor->SetPosition(m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Centroid());
	}
	else
	{
		BuD::Audio::AudioSystem::Play(m_ErrorSoundEffect);
	}
}
