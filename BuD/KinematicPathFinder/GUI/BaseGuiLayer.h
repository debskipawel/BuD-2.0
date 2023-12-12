#pragma once

#include <DataLayers/MainDataLayer.h>

class BaseGuiLayer
{
public:
	BaseGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void = 0;

protected:
	MainDataLayer& m_MainDataLayer;
};
