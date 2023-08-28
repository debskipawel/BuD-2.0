#pragma once

#include <Scene/SceneObjectCAD.h>

#include <Objects/CAD/Intersection/IntersectionPoint.h>

class IntersectionCurve : public SceneObjectCAD
{
public:
	IntersectionCurve(BuD::Scene& scene, std::weak_ptr<SceneObjectCAD> surface, std::vector<IntersectionPoint> intersectionPoints);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void UpdateInstanceData();

	virtual const std::vector<IntersectionPoint>& IntersectionPoints() const { return m_IntersectionPoints; }

	std::weak_ptr<SceneObjectCAD> m_Surface;

protected:
	std::vector<IntersectionPoint> m_IntersectionPoints;

	struct IntersectionCurveSingleSegment
	{
		std::array<dxm::Vector3, 4> m_ControlPoints;
	};

	struct IntersectionCurveInstanceData
	{
		std::vector<IntersectionCurveSingleSegment> m_Segments;
	};

	IntersectionCurveInstanceData m_InstanceData;
};
