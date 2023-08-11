#pragma once

#include <GUI/BaseGuiLayer.h>

class ObjectCreationGuiLayer : public BaseGuiLayer
{
public:
	ObjectCreationGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	void CreateTorus();
	void CreatePoint();

	struct ButtonInfo
	{
		std::string m_Label;
		std::function<void()> m_OnClick;
	};

	std::vector<ButtonInfo> m_Buttons;
};
