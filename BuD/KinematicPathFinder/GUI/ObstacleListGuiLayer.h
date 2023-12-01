#pragma once

#include <GUI/BaseGuiLayer.h>

class ObstacleListGuiLayer : public BaseGuiLayer
{
public:
	ObstacleListGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;
};
