#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class ConsoleGuiLayer : public BaseGuiLayerCAD
{
public:
	ConsoleGuiLayer(MainDataLayerCAD& dataLayer);

	virtual void DrawGui() override;

protected:
	bool m_NewLineAdded = false;
	std::vector<BuD::Log::LogRecord> m_LogRecords;
};
