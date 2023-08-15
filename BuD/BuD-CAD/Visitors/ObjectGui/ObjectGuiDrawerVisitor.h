#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class ObjectGuiDrawerVisitor : public AbstractVisitor
{
public:
	ObjectGuiDrawerVisitor(SceneDataLayer& dataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(BezierCurveC0& curve) override;

protected:
	virtual void DrawGuiForSelectedTransform();
	
	virtual bool DrawGuiForTag(SceneObjectCAD& object);
	virtual bool DrawGuiForTransform(TransformComponent& transform);

	SceneDataLayer& m_SceneDataLayer;
};
