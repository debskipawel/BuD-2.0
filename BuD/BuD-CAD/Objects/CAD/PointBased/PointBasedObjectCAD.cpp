#include "PointBasedObjectCAD.h"

PointBasedObjectCAD::PointBasedObjectCAD(BuD::Scene& scene, std::vector<std::shared_ptr<Point>> controlPoints)
	: SceneObjectCAD(scene), m_ControlPoints(controlPoints)
{
}

void PointBasedObjectCAD::OnPointAdd(std::shared_ptr<Point> controlPoint)
{
	m_ControlPoints.push_back(controlPoint);
}

void PointBasedObjectCAD::OnPointRemove(uint32_t pointId)
{
	std::erase_if(m_ControlPoints, [pointId](std::shared_ptr<Point> controlPoint)
		{
			return controlPoint->Id() == pointId;
		}
	);
}
