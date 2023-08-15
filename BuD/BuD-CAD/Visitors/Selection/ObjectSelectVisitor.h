#pragma once

#include <Visitors/Selection/BaseObjectSelectionVisitor.h>

class ObjectSelectVisitor : public BaseObjectSelectionVisitor
{
public:
	ObjectSelectVisitor(SceneDataLayer& sceneDataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(BezierCurveC0& curve) override;

protected:
	virtual void UniversalSelect(SceneObjectCAD& object);
};
