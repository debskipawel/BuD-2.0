#pragma once

#include <Visitors/Validation/BaseValidationVisitor.h>

class ControlPointValidationVisitor : public BaseValidationVisitor
{
public:
	explicit ControlPointValidationVisitor() = default;
	virtual ~ControlPointValidationVisitor() override = default;

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
};
