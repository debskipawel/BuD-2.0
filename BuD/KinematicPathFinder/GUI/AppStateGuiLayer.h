#pragma once

#include <GUI/BaseGuiLayer.h>

class AppStateGuiLayer: public BaseGuiLayer
{
public:
	AppStateGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
protected:
	struct AppStateButton
	{
		BuD::Texture m_ButtonIcon;
		std::string m_TooltipLabel;
	};

	std::map<AppState, AppStateButton> m_ButtonMap;
};
