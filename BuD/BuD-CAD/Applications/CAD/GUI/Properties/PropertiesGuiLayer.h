#pragma once

#include <memory>

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class PropertiesGuiLayer : public BaseGuiLayerCAD
{
public:
	PropertiesGuiLayer(MainDataLayer& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual bool DrawGuiForTransform(TransformComponent& transform);

	virtual void DrawGuiForSelectedTransform();
	virtual void DrawGuiForSingularObject();
	virtual void DrawDeleteButton();
};
