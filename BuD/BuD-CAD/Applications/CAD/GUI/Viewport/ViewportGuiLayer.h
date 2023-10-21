#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class ViewportGuiLayer : public BaseGuiLayerCAD
{
public:
	ViewportGuiLayer(MainDataLayerCAD& dataLayer);

	virtual void DrawGui() override;
};
