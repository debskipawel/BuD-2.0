#pragma once

#include <GUI/BaseGuiLayer.h>

class ConsoleGuiLayer : public BaseGuiLayer
{
public:
	ConsoleGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;
};
