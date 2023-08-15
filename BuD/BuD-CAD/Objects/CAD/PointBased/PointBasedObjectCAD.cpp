#include "PointBasedObjectCAD.h"

PointBasedObjectCAD::PointBasedObjectCAD(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints)
	: SceneObjectCAD(scene), m_ControlPoints(controlPoints)
{
}

void PointBasedObjectCAD::OnPointAdd(std::weak_ptr<Point> controlPoint)
{
	m_ControlPoints.push_back(controlPoint);

	OnPointModify();
}

void PointBasedObjectCAD::OnPointRemove(uint32_t pointId)
{
	auto removedCount = std::erase_if(m_ControlPoints, [pointId](std::weak_ptr<Point> controlPoint)
		{
			auto controlPointShared = controlPoint.lock();

			return !controlPointShared || controlPointShared->Id() == pointId;
		}
	);

	if (removedCount)
	{
		OnPointModify();
	}
}
