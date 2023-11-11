#include "PointBasedObjectCAD.h"

PointBasedObjectCAD::PointBasedObjectCAD(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: SceneObjectCAD(scene), m_ControlPoints(controlPoints)
{
}
