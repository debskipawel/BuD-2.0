#pragma once

#include <Visitors/Intersection/CalculatorParameterized.h>

class CalculatorPartialDerivativeV : public CalculatorParameterized
{
public:
	virtual void Visit(Torus& torus) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;
};
