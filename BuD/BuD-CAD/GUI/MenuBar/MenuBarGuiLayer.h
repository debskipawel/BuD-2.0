#pragma once

#include <AudioSystem.h>
#include <GUI/BaseGuiLayer.h>

class MenuBarGuiLayer : public BaseGuiLayer
{
public:
	MenuBarGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	struct MainMenuItem
	{
		std::string m_Label;
		std::function<void()> m_Handler;
	};

	virtual void DrawSerializationSettings();
	virtual void DrawRendererSettings();

	std::vector<MainMenuItem> m_MenuItems;
	std::shared_ptr<BuD::Audio::SoundEffect> m_ErrorSoundEffect;
};
