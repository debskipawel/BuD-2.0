#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class PerformanceGuiLayer : public BaseGuiLayerCAD
{
public:
	PerformanceGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;
};
