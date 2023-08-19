#pragma once

#include <Objects/CAD/PointBased/Surfaces/BaseBezierPatch.h>

class BezierPatchC0 : public BaseBezierPatch
{
public:
	BezierPatchC0(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints);

	virtual void Accept(AbstractVisitor& visitor) override;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;
};