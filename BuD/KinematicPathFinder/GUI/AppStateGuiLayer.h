#pragma once

#include <GUI/BaseGuiLayer.h>

class AppStateGuiLayer: public BaseGuiLayer
{
public:
	AppStateGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
