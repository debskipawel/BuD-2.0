#include "CanAddControlPointValidationVisitor.h"

void CanAddControlPointValidationVisitor::Visit(Torus& torus)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(Point& point)
{
	m_Valid = false;
}

void CanAddControlPointValidationVisitor::Visit(BezierCurveC0& curve)
{
	m_Valid = true;
}
