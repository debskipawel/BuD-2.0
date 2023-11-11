#pragma once

#include <functional>

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>

class MenuBarGuiLayerCNC : public BaseGuiLayerCNC
{
public:
	MenuBarGuiLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;

protected:
	struct MainMenuItem
	{
		std::string m_Label;
		std::function<void()> m_Handler;
	};

	std::vector<MainMenuItem> m_MenuItems;
};
