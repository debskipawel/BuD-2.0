#include "PointRemovedVisitor.h"

#include <Visitors/Deletion/ObjectDeletionVisitor.h>

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

void PointRemovedVisitor::Visit(BezierPatchC0& patch)
{
	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectDeletionVisitor>(m_SceneDataLayer);
	visitor->Visit(m_Caller);
}

void PointRemovedVisitor::Visit(BezierPatchC2& patch)
{
	std::unique_ptr<AbstractVisitor> visitor = std::make_unique<ObjectDeletionVisitor>(m_SceneDataLayer);
	visitor->Visit(m_Caller);
}

void PointRemovedVisitor::CommonHandlePointDeletion(BaseCurve& curve)
{
	auto point = m_Point.lock();
	auto pointId = point->Id();

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
