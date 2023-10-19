#pragma once

#include <Applications/CAD/Objects/CAD/PointBased/PointBasedObjectCAD.h>

class BaseCurve : public PointBasedObjectCAD
{
public:
	BaseCurve(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints = {});

	virtual void TogglePolygon(bool borderOn) = 0;
	virtual bool ShouldDisplayPolygon() const;

	struct CubicBezierCurveSegment
	{
		std::array<dxm::Vector3, 4> m_SegmentControlPoints;
		dxm::Vector3 m_Color;
	};

	struct CubicBezierCurveInstanceData
	{
		std::vector<CubicBezierCurveSegment> m_Segments;
	};

	CubicBezierCurveInstanceData m_InstanceData;

protected:
	BuD::MeshDetails LoadCurvePrimitiveMesh();
	BuD::MeshDetails LoadBorderPrimitiveMesh();

	BuD::InstanceComponent CreateCurveInstancingComponent();

	bool m_ControlPointBorder = false;
};