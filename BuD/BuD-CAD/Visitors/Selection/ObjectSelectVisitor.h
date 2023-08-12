#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class ObjectSelectVisitor : public AbstractVisitor
{
public:
	ObjectSelectVisitor(SceneDataLayer& sceneDataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	virtual void UniversalSelect(SceneObjectCAD& object);

	SceneDataLayer& m_SceneDataLayer;
};
