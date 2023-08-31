#pragma once

#include <Objects/CAD/Intersection/IntersectionCurve.h>

class ParameterizedObject2D : public SceneObjectCAD
{
public:
	ParameterizedObject2D(BuD::Scene& scene);

	virtual void AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve);
	virtual void RemoveIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve);

	std::unordered_map<uint32_t, std::weak_ptr<IntersectionCurve>> m_IntersectionCurves;
	std::optional<BuD::EditableTexture> m_ParameterSpace;

protected:
	virtual void DrawIntersectionCurveInParameterSpace(std::shared_ptr<IntersectionCurve> curve);
};
