#pragma once

#include <Objects/CAD/Intersection/IntersectionCurve.h>

class ParameterizedObject2D : public SceneObjectCAD
{
public:
	ParameterizedObject2D(BuD::Scene& scene);

	virtual void AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve);

protected:
	std::vector<std::weak_ptr<IntersectionCurve>> m_IntersectionCurves;
	// Texture m_ParameterSpace;
};
