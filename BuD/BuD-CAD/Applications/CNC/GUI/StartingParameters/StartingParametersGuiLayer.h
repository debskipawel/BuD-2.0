#pragma once

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>

class StartingParametersGuiLayer : public BaseGuiLayerCNC
{
public:
	StartingParametersGuiLayer(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;
};
