#include "PointRemovedVisitor.h"

PointRemovedVisitor::PointRemovedVisitor(SceneDataLayer& dataLayer, std::weak_ptr<Point> point)
	: BasePointActionVisitor(dataLayer, point)
{
}

void PointRemovedVisitor::Visit(BezierCurveC0& curve)
{
	CommonHandlePointDeletion(curve);
}

void PointRemovedVisitor::Visit(BezierCurveC2& curve)
{
	CommonHandlePointDeletion(curve);
}

void PointRemovedVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	CommonHandlePointDeletion(curve);
}

void PointRemovedVisitor::CommonHandlePointDeletion(BaseCurve& curve)
{
	auto pointId = m_Point->Id();

	auto removedCount = std::erase_if(curve.m_ControlPoints, [pointId](std::weak_ptr<Point> controlPoint)
		{
			auto controlPointShared = controlPoint.lock();

			return !controlPointShared || controlPointShared->Id() == pointId;
		}
	);

	if (removedCount)
	{
		curve.OnPointModify();
	}
}
