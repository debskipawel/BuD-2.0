#pragma once

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>

class MillingToolGuiLayerCNC : public BaseGuiLayerCNC
{
public:
	MillingToolGuiLayerCNC(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;
};
