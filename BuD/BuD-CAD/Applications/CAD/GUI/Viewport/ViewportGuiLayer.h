#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class ViewportGuiLayer : public BaseGuiLayerCAD
{
public:
	ViewportGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;
};
