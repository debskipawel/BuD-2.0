#pragma once

#include <GUI/BaseGuiLayer.h>

class SimulationParametersGuiLayer : public BaseGuiLayer
{
public:
	SimulationParametersGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
