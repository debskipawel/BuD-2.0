#pragma once

#include <Behaviors/Keyboard/BaseKeyboardBehaviorLayer.h>

#include <AudioSystem.h>

class ShortcutsKeyboardBehaviorLayer : public BaseKeyboardBehaviorLayer
{
public:
	ShortcutsKeyboardBehaviorLayer(MainDataLayer& dataLayer);

	virtual bool OnKeyPress(BuD::KeyboardKeys key) override;
	virtual bool OnKeyRelease(BuD::KeyboardKeys key) override;

protected:
	virtual void Undo();
	virtual void Redo();

	std::unordered_map<BuD::KeyboardKeys, bool> m_SystemKeysPressed;
	std::shared_ptr<BuD::Audio::SoundEffect> m_ErrorSoundEffect;
};
