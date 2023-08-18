#pragma once

#include <Visitors/PointBased/BasePointActionVisitor.h>

class PointRemovedVisitor : public BasePointActionVisitor
{
public:
	PointRemovedVisitor(SceneDataLayer& dataLayer, std::weak_ptr<Point> pointAdded);

	virtual void Visit(BezierCurveC0& curve);
	virtual void Visit(BezierCurveC2& curve);
	virtual void Visit(YukselInterpolatingCurveC2& curve);

protected:
	virtual void CommonHandlePointDeletion(BaseCurve& curve);
};
