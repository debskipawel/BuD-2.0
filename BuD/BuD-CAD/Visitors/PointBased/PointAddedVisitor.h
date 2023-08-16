#pragma once

#include <DataLayers/MainDataLayer.h>

#include <Visitors/AbstractVisitor.h>

class PointAddedVisitor : public AbstractVisitor
{
public:
	PointAddedVisitor(MainDataLayer& dataLayer, std::weak_ptr<Point> pointAdded);

	virtual void Visit(BezierCurveC0& curve);
	virtual void Visit(BezierCurveC2& curve);

protected:
	MainDataLayer& m_MainDataLayer;
	std::shared_ptr<Point> m_PointAdded;
};
