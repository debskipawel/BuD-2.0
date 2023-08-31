#pragma once

#include <BuD.h>

#include <GUI/BaseGuiLayer.h>

class AppStateGuiLayer : public BaseGuiLayer
{
public:
	AppStateGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	struct AppStateButton
	{
		BuD::Texture m_ButtonIcon;
		std::string m_TooltipLabel;
	};

	std::map<AppState, AppStateButton> m_ButtonMap;
};
