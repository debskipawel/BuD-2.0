#pragma once

#include <Visitors/Selection/BaseObjectSelectionVisitor.h>

class ObjectSelectVisitor : public BaseObjectSelectionVisitor
{
public:
	ObjectSelectVisitor(SceneDataLayer& sceneDataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	virtual void UniversalSelect(SceneObjectCAD& object);
};
