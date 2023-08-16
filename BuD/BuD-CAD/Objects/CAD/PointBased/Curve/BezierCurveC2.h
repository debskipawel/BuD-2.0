#pragma once

#include <Objects/CAD/PointBased/Curve/BaseCubicBezierCurve.h>

class BezierCurveC2 : public BaseCubicBezierCurve
{
public:
	BezierCurveC2(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints = {});

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void OnPointModify() override;

	virtual void RenderControlPointBorder(bool borderOn) override;

	dxm::Vector3 m_Color = dxm::Vector3::One;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;
};
