#pragma once

#include <DataLayers/MainDataLayer.h>

#include <Visitors/AbstractVisitor.h>

class PointAddedVisitor : public AbstractVisitor
{
public:
	PointAddedVisitor(MainDataLayer& dataLayer, std::weak_ptr<Point> pointAdded);

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

	virtual void Visit(BezierCurveC0& curve);
	virtual void Visit(BezierCurveC2& curve);
	virtual void Visit(YukselInterpolatingCurveC2& curve);

protected:
	virtual void UpdateCentroidCursor();

	MainDataLayer& m_MainDataLayer;
	std::shared_ptr<Point> m_PointAdded;
};
