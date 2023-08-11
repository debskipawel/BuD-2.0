#pragma once

#include <GUI/BaseGuiLayer.h>

class ObjectListGuiLayer : public BaseGuiLayer
{
public:
	ObjectListGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual void DrawGuiForFilters();
};
