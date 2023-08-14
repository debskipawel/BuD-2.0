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

	virtual void DrawSerializationSettings();
	virtual void DrawRendererSettings();

	virtual void DrawGuiForPopups();

	std::vector<MainMenuItem> m_MenuItems;
};
