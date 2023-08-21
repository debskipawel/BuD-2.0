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

	virtual void DrawFileSettings();
	virtual void DrawEditSettings();
	virtual void DrawRendererSettings();

	virtual void DrawMultiEyeSettingsPopup();

	virtual void OpenSaveSceneDialog();
	virtual void DrawSaveSceneDialog();
	virtual void Save();

	std::vector<MainMenuItem> m_MenuItems;
	std::vector<RendererModeMenuItem> m_RendererModeMenuItems;

	bool m_MultiEyeSettingsPopupOpen;
};
