#pragma once

#include <GUI/BaseGuiLayer.h>

class PerformanceGuiLayer : public BaseGuiLayer
{
public:
	PerformanceGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;
};
