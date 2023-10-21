#pragma once

#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>
#include <GUI/BaseGuiLayer.h>

class BaseGuiLayerCAD : public BaseGuiLayer
{
public:
	BaseGuiLayerCAD(MainDataLayerCAD& dataLayer);
	virtual ~BaseGuiLayerCAD() = default;

protected:
	MainDataLayerCAD& m_MainDataLayer;
};
