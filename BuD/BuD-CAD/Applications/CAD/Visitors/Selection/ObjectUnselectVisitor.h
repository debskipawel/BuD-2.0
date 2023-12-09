#pragma once

#include <Applications/CAD/Visitors/Selection/BaseObjectSelectionVisitor.h>

class ObjectUnselectVisitor : public BaseObjectSelectionVisitor
{
public:
	ObjectUnselectVisitor(SceneDataLayerCAD& sceneDataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(InfinitePlane& plane) override;
	virtual void Visit(FinitePlane& plane) override;

	virtual void Visit(IntersectionCurve& curve) override;

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
	virtual void Visit(YukselInterpolatingCurveC2& curve) override;

	virtual void Visit(BezierPatchC0& patch) override;
	virtual void Visit(BezierPatchC2& patch) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

protected:
	virtual void CommonUnselectCurve(BaseCurve& curve);
	virtual void CommonUnselectPatch(BaseBezierPatch& patch);
	virtual void CommonUnselectSurface(BaseBezierSurface& surface);

	virtual void UnselectManually(std::weak_ptr<SceneObjectCAD> object);
	virtual void UnselectForTransform(std::weak_ptr<SceneObjectCAD> object);
};
