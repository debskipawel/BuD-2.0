#pragma once

#include <Visitors/Intersection/ParameterizedCalculator.h>

class PointOnSurfaceVisitor : public ParameterizedCalculator
{
public:
	virtual void Visit(Torus& torus) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;
};
