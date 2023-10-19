#pragma once

#include <Applications/CAD/DataLayers/MainDataLayer.h>
#include <GUI/BaseGuiLayer.h>

class BaseGuiLayerCAD : public BaseGuiLayer
{
public:
	BaseGuiLayerCAD(MainDataLayer& dataLayer);
	virtual ~BaseGuiLayerCAD() = default;

protected:
	MainDataLayer& m_MainDataLayer;
};
