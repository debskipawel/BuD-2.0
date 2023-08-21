#include "ControlPointValidationVisitor.h"

void ControlPointValidationVisitor::Visit(Torus& torus)
{
	m_Valid = false;
}

void ControlPointValidationVisitor::Visit(Point& point)
{
	m_Valid = true;
}

void ControlPointValidationVisitor::Visit(BezierCurveC0& curve)
{
	m_Valid = false;
}

void ControlPointValidationVisitor::Visit(BezierCurveC2& curve)
{
	m_Valid = false;
}

void ControlPointValidationVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	m_Valid = false;
}

void ControlPointValidationVisitor::Visit(BezierPatchC0& patch)
{
	m_Valid = false;
}

void ControlPointValidationVisitor::Visit(BezierPatchC2& patch)
{
	m_Valid = false;
}

void ControlPointValidationVisitor::Visit(BezierSurfaceC0& surface)
{
	m_Valid = false;
}

void ControlPointValidationVisitor::Visit(BezierSurfaceC2& surface)
{
	m_Valid = false;
}
