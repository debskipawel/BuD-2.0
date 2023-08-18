#pragma once

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

	struct RendererModeMenuItem
	{
		std::string m_Label;
		BuD::RenderingMode m_RenderingMode;
	};

	virtual void DrawSerializationSettings();
	virtual void DrawRendererSettings();

	std::vector<MainMenuItem> m_MenuItems;
	std::vector<RendererModeMenuItem> m_RendereModeMenuItems;
};
