#pragma once

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>
#include <Applications/CAD/Filters/FilterInfo.h>

class ObjectListGuiLayer : public BaseGuiLayerCAD
{
public:
	ObjectListGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual void DrawListFiltered();
	virtual void DrawListClipped();

	virtual void DrawGuiForFilters();

	FilterInfo m_Filter;
};
