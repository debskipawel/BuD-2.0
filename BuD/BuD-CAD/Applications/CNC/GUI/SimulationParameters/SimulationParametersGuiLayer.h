#pragma once

#include <Applications/CNC/GUI/BaseGuiLayerCNC.h>

class SimulationParametersGuiLayer : public BaseGuiLayerCNC
{
public:
	SimulationParametersGuiLayer(MainDataLayerCNC& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual void DrawParameters();
	virtual void DrawStartAndSkipButtons();
};
