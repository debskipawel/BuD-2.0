#pragma once

#include <memory>

#include <Applications/CAD/GUI/BaseGuiLayerCAD.h>

class PropertiesGuiLayer : public BaseGuiLayerCAD
{
public:
	PropertiesGuiLayer(MainDataLayerCAD& dataLayer);

	virtual void DrawGui() override;

protected:
	virtual bool DrawGuiForTransform(TransformComponent& transform);

	virtual void DrawGuiForSelectedTransform();
	virtual void DrawGuiForSingularObject();
	virtual void DrawDeleteButton();

	virtual void DrawGuiForOffsetSurface(std::weak_ptr<SceneObjectCAD> object);

	float m_SurfaceOffset;
};
