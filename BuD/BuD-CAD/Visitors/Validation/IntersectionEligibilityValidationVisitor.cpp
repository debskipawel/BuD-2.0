#include "IntersectionEligibilityValidationVisitor.h"

void IntersectionEligibilityValidationVisitor::Visit(Torus& torus)
{
	m_Valid = true;
}

void IntersectionEligibilityValidationVisitor::Visit(Point& point)
{
	m_Valid = false;
}

void IntersectionEligibilityValidationVisitor::Visit(IntersectionCurve& curve)
{
	m_Valid = false;
}

void IntersectionEligibilityValidationVisitor::Visit(BezierCurveC0& curve)
{
	m_Valid = false;
}

void IntersectionEligibilityValidationVisitor::Visit(BezierCurveC2& curve)
{
	m_Valid = false;
}

void IntersectionEligibilityValidationVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	m_Valid = false;
}

void IntersectionEligibilityValidationVisitor::Visit(BezierPatchC0& patch)
{
	m_Valid = false;
}

void IntersectionEligibilityValidationVisitor::Visit(BezierPatchC2& patch)
{
	m_Valid = false;
}

void IntersectionEligibilityValidationVisitor::Visit(BezierSurfaceC0& surface)
{
	m_Valid = true;
}

void IntersectionEligibilityValidationVisitor::Visit(BezierSurfaceC2& surface)
{
	m_Valid = true;
}
