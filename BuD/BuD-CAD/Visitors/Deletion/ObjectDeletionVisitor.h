#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class ObjectDeletionVisitor : public AbstractVisitor
{
public:
	ObjectDeletionVisitor(SceneDataLayer& dataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	SceneDataLayer& m_SceneDataLayer;
};
