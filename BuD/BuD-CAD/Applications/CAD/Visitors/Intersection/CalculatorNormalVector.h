#pragma once

#include <memory>

#include <Applications/CAD/Visitors/Intersection/CalculatorParameterized.h>

class CalculatorNormalVector : public CalculatorParameterized
{
public:
	explicit CalculatorNormalVector();

	virtual void Visit(Torus& torus) override;

	virtual void Visit(InfinitePlane& plane) override;
	virtual void Visit(FinitePlane& plane) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

	virtual void Visit(OffsetSurface& surface) override;
};
