#pragma once

#include <Visitors/Selection/BaseObjectSelectionVisitor.h>

class ObjectSelectVisitor : public BaseObjectSelectionVisitor
{
public:
	ObjectSelectVisitor(SceneDataLayer& sceneDataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
	virtual void Visit(YukselInterpolatingCurveC2& curve) override;

	virtual void Visit(BezierPatchC0& patch) override;
	virtual void Visit(BezierPatchC2& patch) override;

protected:
	virtual void CommonSelectCurve(BaseCurve& curve);
	virtual void CommonSelectPatch(BaseBezierPatch& patch);

	virtual void SelectManually(std::weak_ptr<SceneObjectCAD> object);
	virtual void SelectForTransform(std::weak_ptr<SceneObjectCAD> object);
};
