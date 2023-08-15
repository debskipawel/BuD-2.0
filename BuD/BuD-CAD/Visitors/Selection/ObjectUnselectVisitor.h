#pragma once

#include <Visitors/Selection/BaseObjectSelectionVisitor.h>

class ObjectUnselectVisitor : public BaseObjectSelectionVisitor
{
public:
	ObjectUnselectVisitor(SceneDataLayer& sceneDataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(BezierCurveC0& curve) override;

protected:
	virtual void UniversalUnselect(SceneObjectCAD& object);
};
