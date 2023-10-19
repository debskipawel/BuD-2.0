#include "FilterValidationVisitor.h"

FilterValidationVisitor::FilterValidationVisitor(const FilterInfo& filterInfo)
	: m_FilterInfo(filterInfo)
{
}

void FilterValidationVisitor::Visit(Torus& torus)
{
	m_Valid = ValidateTag(torus);
}

void FilterValidationVisitor::Visit(Point& point)
{
	if (m_FilterInfo.m_SkipPoints)
	{
		m_Valid = false;
		return;
	}

	m_Valid = ValidateTag(point);
}

void FilterValidationVisitor::Visit(IntersectionCurve& curve)
{
	m_Valid = ValidateTag(curve);
}

void FilterValidationVisitor::Visit(BezierCurveC0& curve)
{
	m_Valid = ValidateTag(curve);
}

void FilterValidationVisitor::Visit(BezierCurveC2& curve)
{
	m_Valid = ValidateTag(curve);
}

void FilterValidationVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	m_Valid = ValidateTag(curve);
}

void FilterValidationVisitor::Visit(BezierPatchC0& patch)
{
	m_Valid = ValidateTag(patch);
}

void FilterValidationVisitor::Visit(BezierPatchC2& patch)
{
	m_Valid = ValidateTag(patch);
}

void FilterValidationVisitor::Visit(BezierSurfaceC0& surface)
{
	m_Valid = ValidateTag(surface);
}

void FilterValidationVisitor::Visit(BezierSurfaceC2& surface)
{
	m_Valid = ValidateTag(surface);
}

bool FilterValidationVisitor::ValidateTag(SceneObjectCAD& object)
{
	if (m_FilterInfo.m_TagFilter.empty())
	{
		return true;
	}

	return object.m_Tag.find(m_FilterInfo.m_TagFilter) != std::string::npos;
}
