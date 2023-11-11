#pragma once

#include <Applications/CAD/Objects/Point.h>

class PointBasedObjectCAD : public SceneObjectCAD
{
public:
	PointBasedObjectCAD(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints = {});

	virtual void OnPointModify() = 0;

	std::vector<std::weak_ptr<Point>> m_ControlPoints;
};
