#pragma once

#include <Applications/CAD/Visitors/PointBased/BasePointActionVisitor.h>

class PointRemovedVisitor : public BasePointActionVisitor
{
public:
	PointRemovedVisitor(SceneDataLayerCAD& dataLayer, std::weak_ptr<Point> pointAdded);

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
	virtual void Visit(YukselInterpolatingCurveC2& curve) override;

	virtual void Visit(BezierPatchC0& patch) override;
	virtual void Visit(BezierPatchC2& patch) override;

protected:
	virtual void CommonHandlePointDeletion(BaseCurve& curve);
};
