#pragma once

#include <DataLayers/MainDataLayer.h>

class BaseGuiLayer
{
public:
	BaseGuiLayer(MainDataLayer& dataLayer);
	virtual ~BaseGuiLayer() = default;

	virtual void DrawGui() = 0;

protected:
	MainDataLayer& m_MainDataLayer;
};
