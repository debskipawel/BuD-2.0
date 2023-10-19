#pragma once

#include <BuD.h>

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class AppStateGuiLayer : public BaseGuiLayerCAD
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
