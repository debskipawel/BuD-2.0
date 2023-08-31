#pragma once

#include <GUI/BaseGuiLayer.h>
#include <Filters/FilterInfo.h>

class ObjectListGuiLayer : public BaseGuiLayer
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
