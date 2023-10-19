#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class ObjectCreationGuiLayer : public BaseGuiLayerCAD
{
public:
	ObjectCreationGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	void CreateTorus();
	void CreatePoint();

	void OpenPopupForSurfaceCreationC0();
	void OpenPopupForSurfaceCreationC2();

	void DrawGuiForSurfaceCreationC0();
	void DrawGuiForSurfaceCreationC2();

	struct SurfaceCreationParameters
	{
		SurfaceCreationParameters();
		SurfaceCreationParameters(dxm::Vector3 position);

		dxm::Vector3 m_Position;
		int m_PatchesU, m_PatchesV;
		bool m_Cylinder;
	};

	struct ButtonInfo
	{
		std::string m_Label;
		std::function<void()> m_OnClick;
	};

	bool m_OpenPopupForSurfaceC0;
	bool m_OpenPopupForSurfaceC2;

	SurfaceCreationParameters m_SurfaceParametersC0;
	SurfaceCreationParameters m_SurfaceParametersC2;

	std::vector<ButtonInfo> m_Buttons;
};
