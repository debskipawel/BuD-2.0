#pragma once

#include <DataLayers/MainDataLayer.h>

class BaseGuiLayer
{
public:
	BaseGuiLayer(MainDataLayer& dataLayer);
	virtual ~BaseGuiLayer() = default;

	virtual auto DrawGui() -> void = 0;

protected:
	MainDataLayer& m_MainDataLayer;
};
