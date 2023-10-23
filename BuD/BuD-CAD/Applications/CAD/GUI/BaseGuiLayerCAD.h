#pragma once

#include <Applications/CAD/DataLayers/MainDataLayerCAD.h>
#include <Applications/Shared/GUI/BaseGuiLayer.h>

class BaseGuiLayerCAD : public BaseGuiLayer
{
public:
	BaseGuiLayerCAD(MainDataLayerCAD& dataLayer);
	virtual ~BaseGuiLayerCAD() = default;

protected:
	MainDataLayerCAD& m_MainDataLayer;
};
