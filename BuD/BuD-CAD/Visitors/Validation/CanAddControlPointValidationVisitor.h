#pragma once

#include <Visitors/Validation/BaseValidationVisitor.h>

class CanAddControlPointValidationVisitor : public BaseValidationVisitor
{
public:
	CanAddControlPointValidationVisitor() = default;

	void Visit(Torus& torus);
	void Visit(Point& point);

	void Visit(BezierCurveC0& curve);
};
