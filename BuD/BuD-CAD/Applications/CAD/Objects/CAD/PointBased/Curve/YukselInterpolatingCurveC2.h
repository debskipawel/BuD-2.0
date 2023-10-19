#pragma once

#include <Applications/CAD/Objects/CAD/PointBased/Curve/BaseCurve.h>

class YukselInterpolatingCurveC2 : public BaseCurve
{
public:
	YukselInterpolatingCurveC2(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints = {});

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void OnPointModify() override;

	virtual void TogglePolygon(bool borderOn) override;

	dxm::Vector3 m_Color = dxm::Vector3::One;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;

protected:
	struct YukselCurveSingleSegment
	{
		std::array<dxm::Vector3, 3> m_ControlPoints;
	};

	struct YukselCurveSingleSegmentsInstanceData
	{
		std::vector<YukselCurveSingleSegment> m_Segments;
	};

	YukselCurveSingleSegmentsInstanceData m_SingleSegmentsInstanceData;
};
