#include "CanAddControlPointValidationVisitor.h"

void CanAddControlPointValidationVisitor::Visit(Torus& torus)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(Point& point)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(IntersectionCurve& curve)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(BezierCurveC0& curve)
{
	m_Valid = true;
}

void CanAddControlPointValidationVisitor::Visit(BezierCurveC2& curve)
{
	m_Valid = true;
}

void CanAddControlPointValidationVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	m_Valid = true;
}

void CanAddControlPointValidationVisitor::Visit(BezierPatchC0& patch)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(BezierPatchC2& patch)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(BezierSurfaceC0& surface)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(BezierSurfaceC2& surface)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(OffsetSurface& surface)
{
	m_Valid = false;
}
