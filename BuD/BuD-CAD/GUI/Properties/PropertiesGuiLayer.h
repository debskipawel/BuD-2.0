#pragma once

#include <memory>

#include <GUI/BaseGuiLayer.h>

class PropertiesGuiLayer : public BaseGuiLayer
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
