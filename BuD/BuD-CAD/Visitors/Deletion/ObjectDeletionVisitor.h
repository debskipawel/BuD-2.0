#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <Visitors/AbstractVisitor.h>

class ObjectDeletionVisitor : public AbstractVisitor
{
public:
	ObjectDeletionVisitor(SceneDataLayer& dataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
	virtual void Visit(YukselInterpolatingCurveC2& curve) override;

protected:
	virtual void NotifyControlPointsAboutDeletion(PointBasedObjectCAD& object);

	SceneDataLayer& m_SceneDataLayer;
};
