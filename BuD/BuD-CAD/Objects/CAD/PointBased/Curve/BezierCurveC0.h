#pragma once

#include <Objects/CAD/PointBased/PointBasedObjectCAD.h>

class BezierCurveC0 : public PointBasedObjectCAD
{
public:
	BezierCurveC0(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints = {});

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void OnPointModify() override;

	virtual void RenderBorder(bool borderOn);
	virtual bool RenderBorder() const;

	dxm::Vector3 m_Color = dxm::Vector3::One;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;

	struct BezierCurveSegmentC0
	{
		std::array<dxm::Vector3, 4> m_SegmentControlPoints;
		dxm::Vector3 m_Color;
	};

	struct InstanceDataBezierCurveC0
	{
		std::vector<BezierCurveSegmentC0> m_Segments;
	};

	InstanceDataBezierCurveC0 m_InstanceData;

protected:
	bool m_Border = false;
};
