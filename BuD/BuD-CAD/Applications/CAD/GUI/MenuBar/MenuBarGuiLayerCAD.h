#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class MenuBarGuiLayerCAD : public BaseGuiLayerCAD
{
public:
	MenuBarGuiLayerCAD(MainDataLayerCAD& dataLayer);

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
	virtual void DrawMillingPathsMenu();

	virtual void ResetMillingParametersToDefault();

	virtual void DrawMultiEyeSettingsPopup();
	virtual void DrawGenerateMillingToolPathsPopup();

	virtual void OpenSaveSceneDialog();
	virtual void DrawSaveSceneDialog();

	virtual void OpenLoadSceneDialog();
	virtual void DrawLoadSceneDialog();

	std::vector<MainMenuItem> m_MenuItems;
	std::vector<RendererModeMenuItem> m_RendererModeMenuItems;

	bool m_MultiEyeSettingsPopupOpen;
	bool m_GenerateMillingPathsPopupOpen;

	dxm::Vector3 m_MilledMaterialSize;
	float m_MilledModelPlaneHeight;
};
