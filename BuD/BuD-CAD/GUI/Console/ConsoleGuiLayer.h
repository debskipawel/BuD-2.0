#pragma once

#include <GUI/BaseGuiLayer.h>

class ConsoleGuiLayer : public BaseGuiLayer
{
public:
	ConsoleGuiLayer();

	virtual void DrawGui() override;
};
