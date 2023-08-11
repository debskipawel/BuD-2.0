#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class ObjectUnselectVisitor : public AbstractVisitor
{
public:
	ObjectUnselectVisitor(SceneDataLayer& sceneDataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	SceneDataLayer& m_SceneDataLayer;
};
