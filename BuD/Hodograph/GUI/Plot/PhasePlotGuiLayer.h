#pragma once

#include <GUI/BaseGuiLayer.h>

class PhasePlotGuiLayer : public BaseGuiLayer
{
public:
	PhasePlotGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
