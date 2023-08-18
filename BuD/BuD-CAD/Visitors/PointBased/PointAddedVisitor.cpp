#include "PointAddedVisitor.h"

PointAddedVisitor::PointAddedVisitor(SceneDataLayer& dataLayer, std::weak_ptr<Point> pointAdded)
	: BasePointActionVisitor(dataLayer, pointAdded)
{
}

void PointAddedVisitor::Visit(BezierCurveC0& curve)
{
	CommonHandlePointAddition(curve);
}

void PointAddedVisitor::Visit(BezierCurveC2& curve)
{
	CommonHandlePointAddition(curve);
}

void PointAddedVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	CommonHandlePointAddition(curve);
}

void PointAddedVisitor::CommonHandlePointAddition(BaseCurve& curve)
{
	curve.m_ControlPoints.push_back(m_Point);

	curve.OnPointModify();

	auto pointBasedObject = std::dynamic_pointer_cast<PointBasedObjectCAD>(m_Caller.lock());

	m_Point->m_PointBasedObjects.push_back(pointBasedObject);

	UpdateCentroidCursor();
}
