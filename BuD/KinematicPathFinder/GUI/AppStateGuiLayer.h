#pragma once

#include <GUI/BaseGuiLayer.h>

class AppStateGuiLayer: public BaseGuiLayer
{
public:
	AppStateGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	virtual auto RunSimulation() -> bool;

	struct AppStateButton
	{
		BuD::Texture m_ButtonIcon;
		std::string m_TooltipLabel;

		std::function<bool()> m_OnClick = []() { return true; };
	};

	std::map<AppState, AppStateButton> m_ButtonMap;
};
