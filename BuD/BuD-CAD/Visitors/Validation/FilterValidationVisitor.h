#pragma once

#include <Filters/FilterInfo.h>
#include <Visitors/Validation/BaseValidationVisitor.h>

class FilterValidationVisitor : public BaseValidationVisitor
{
public:
	FilterValidationVisitor(const FilterInfo& filterInfo);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(IntersectionCurve& curve) override;

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
	virtual void Visit(YukselInterpolatingCurveC2& curve) override;

	virtual void Visit(BezierPatchC0& patch) override;
	virtual void Visit(BezierPatchC2& patch) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

protected:
	virtual bool ValidateTag(SceneObjectCAD& object);

	FilterInfo m_FilterInfo;
};
