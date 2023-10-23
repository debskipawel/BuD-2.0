#pragma once

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>

class ViewportGuiLayerCNC : public BaseGuiLayerCNC
{
public:
	ViewportGuiLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;
};
