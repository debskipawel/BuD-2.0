#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class ViewportGuiLayerCAD : public BaseGuiLayerCAD
{
public:
	ViewportGuiLayerCAD(MainDataLayerCAD& dataLayer);

	virtual void DrawGui() override;
};
