#pragma once

#include <BuD.h>

#include <Applications/Shared/GUI/BaseGuiLayer.h>

class ConsoleGuiLayer : public BaseGuiLayer
{
public:
	ConsoleGuiLayer(std::string panelId);

	virtual void DrawGui() override;

protected:
	std::string m_PanelId;

	bool m_NewLineAdded = false;
	std::vector<BuD::Log::LogRecord> m_LogRecords;
};
