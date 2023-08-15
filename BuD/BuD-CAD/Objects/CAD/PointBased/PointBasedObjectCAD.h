#pragma once

#include <Objects/CAD/Point.h>

class PointBasedObjectCAD : public SceneObjectCAD
{
public:
	PointBasedObjectCAD(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints = {});

	virtual void OnPointAdd(std::weak_ptr<Point> controlPoint);
	virtual void OnPointRemove(uint32_t pointId);
	virtual void OnPointModify() = 0;

	std::vector<std::weak_ptr<Point>> m_ControlPoints;
};
