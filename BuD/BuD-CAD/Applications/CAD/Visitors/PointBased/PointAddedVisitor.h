#pragma once

#include <Applications/CAD/Visitors/PointBased/BasePointActionVisitor.h>

class PointAddedVisitor : public BasePointActionVisitor
{
public:
	PointAddedVisitor(SceneDataLayer& dataLayer, std::weak_ptr<Point> pointAdded);

	virtual void Visit(BezierCurveC0& curve);
	virtual void Visit(BezierCurveC2& curve);
	virtual void Visit(YukselInterpolatingCurveC2& curve);

protected:
	virtual void CommonHandlePointAddition(BaseCurve& curve);
};
