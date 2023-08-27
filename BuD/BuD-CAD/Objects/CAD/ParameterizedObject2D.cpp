#include "ParameterizedObject2D.h"

ParameterizedObject2D::ParameterizedObject2D(BuD::Scene& scene)
	: SceneObjectCAD(scene), m_ParameterSpace(std::nullopt)
{
}

void ParameterizedObject2D::AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve)
{
	auto intersectionCurveShared = intersectionCurve.lock();

	if (!intersectionCurveShared)
	{
		return;
	}

	m_IntersectionCurves.emplace(intersectionCurveShared->Id(), intersectionCurve);

	if (!m_ParameterSpace.has_value())
	{
		m_ParameterSpace = BuD::EditableTexture(1024, 1024);
	}

	// Draw curve in parameter space
	m_ParameterSpace->BeginEdit();

	m_ParameterSpace->Clear({ 1.0f, 1.0f, 0.0f, 1.0f });

	m_ParameterSpace->EndEdit();
}
