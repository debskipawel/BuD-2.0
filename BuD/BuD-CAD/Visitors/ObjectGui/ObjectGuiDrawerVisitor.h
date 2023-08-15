#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class ObjectGuiDrawerVisitor : public AbstractVisitor
{
public:
	ObjectGuiDrawerVisitor(SceneDataLayer& dataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	virtual bool DrawGuiForTag(SceneObjectCAD& object);
	virtual bool DrawGuiForTransform(SceneObjectCAD& object);

	SceneDataLayer& m_SceneDataLayer;
};
