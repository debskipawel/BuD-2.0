#pragma once

#include <Visitors/Validation/BaseValidationVisitor.h>

class CanAddControlPointValidationVisitor : public BaseValidationVisitor
{
public:
	CanAddControlPointValidationVisitor() = default;

	void Visit(Torus& torus) override;
	void Visit(Point& point) override;

	void Visit(BezierCurveC0& curve) override;
	void Visit(BezierCurveC2& curve) override;
	void Visit(YukselInterpolatingCurveC2& curve) override;

	void Visit(BezierPatchC0& patch) override;
	void Visit(BezierPatchC2& patch) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	//virtual void Visit(BezierSurfaceC2& surface) override;
};
