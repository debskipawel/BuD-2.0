#pragma once

#include <string>

#include <Applications/Shared/GUI/BaseGuiLayer.h>

class PerformanceGuiLayer : public BaseGuiLayer
{
public:
	PerformanceGuiLayer(std::string panelId = "###performance");

	virtual void DrawGui() override;

protected:
	std::string m_PanelId;
};
