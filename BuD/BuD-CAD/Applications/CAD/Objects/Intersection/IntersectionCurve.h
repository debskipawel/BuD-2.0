#pragma once

#include <Applications/CAD/Scene/SceneObjectCAD.h>

#include <Applications/CAD/Intersection/ResultConverter/IntersectionPoint.h>

class IntersectionCurve : public SceneObjectCAD
{
public:
	IntersectionCurve(BuD::Scene& scene, std::weak_ptr<SceneObjectCAD> surface, std::vector<IntersectionPoint> intersectionPoints);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void UpdateInstanceData();

	virtual const std::vector<IntersectionPoint>& IntersectionPoints() const { return m_IntersectionPoints; }

	dxm::Vector3 m_Color;

	std::weak_ptr<SceneObjectCAD> m_Surface;

	struct IntersectionCurveSingleSegment
	{
		std::array<dxm::Vector3, 4> m_ControlPoints;
		dxm::Vector3 m_Color;
	};

	struct IntersectionCurveInstanceData
	{
		std::vector<IntersectionCurveSingleSegment> m_Segments;
	};

	IntersectionCurveInstanceData m_InstanceData;

	inline static dxm::Vector3 SELECTED_COLOR = { 0.0f, 0.5f, 1.0f };
	inline static dxm::Vector3 UNSELECTED_COLOR = { 1.0f, 0.0f, 0.0f };

protected:
	std::vector<IntersectionPoint> m_IntersectionPoints;
};
