#pragma once

#include <GUI/BaseGuiLayer.h>

class ConsoleGuiLayer : public BaseGuiLayer
{
public:
	ConsoleGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	bool m_NewLineAdded = false;
	std::vector<BuD::Log::LogRecord> m_LogRecords;
};
