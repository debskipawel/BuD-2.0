#include "ParameterizedObject2D.h"

ParameterizedObject2D::ParameterizedObject2D(BuD::Scene& scene)
	: SceneObjectCAD(scene)
{
}

void ParameterizedObject2D::AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve)
{
	m_IntersectionCurves.push_back(intersectionCurve);
}
