#pragma once

#include <GUI/BaseGuiLayer.h>

class PerformanceGuiLayer : public BaseGuiLayer
{
public:
	PerformanceGuiLayer();

	virtual void DrawGui() override;
};
