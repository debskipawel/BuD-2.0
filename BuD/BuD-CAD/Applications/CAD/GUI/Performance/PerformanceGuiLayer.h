#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class PerformanceGuiLayer : public BaseGuiLayerCAD
{
public:
	PerformanceGuiLayer(MainDataLayerCAD& dataLayer);

	virtual void DrawGui() override;
};
