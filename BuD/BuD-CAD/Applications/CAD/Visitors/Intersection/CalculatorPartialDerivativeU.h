#pragma once

#include <Applications/CAD/Visitors/Intersection/CalculatorParameterized.h>

class CalculatorPartialDerivativeU : public CalculatorParameterized
{
public:
	virtual void Visit(Torus& torus) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

	virtual void Visit(OffsetSurface& surface) override;
};
