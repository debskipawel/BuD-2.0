#pragma once

#include <Applications/CAD/Objects/PointBased/Curve/BaseCurve.h>

class BezierCurveC0 : public BaseCurve
{
public:
	BezierCurveC0(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints = {});

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void OnPointModify() override;

	virtual void TogglePolygon(bool borderOn) override;

	dxm::Vector3 m_Color = dxm::Vector3::One;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;
};
