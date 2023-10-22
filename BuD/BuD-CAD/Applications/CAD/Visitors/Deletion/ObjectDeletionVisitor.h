#pragma once

#include <Applications/CAD/DataLayers/SceneDataLayerCAD.h>
#include <Applications/CAD/Visitors/AbstractVisitor.h>

class ObjectDeletionVisitor : public AbstractVisitor
{
public:
	ObjectDeletionVisitor(SceneDataLayerCAD& dataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(IntersectionCurve& curve) override;

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
	virtual void Visit(YukselInterpolatingCurveC2& curve) override;

	virtual void Visit(BezierPatchC0& patch) override;
	virtual void Visit(BezierPatchC2& patch) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

protected:
	virtual void NotifyControlPointsAboutDeletion(PointBasedObjectCAD& object);

	SceneDataLayerCAD& m_SceneDataLayer;
};