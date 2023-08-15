#include "SceneCAD.h"

#include <Objects/CAD/Torus.h>
#include <Objects/CAD/Point.h>

#include <Objects/CAD/PointBased/Curve/BezierCurveC0.h>

SceneCAD::SceneCAD()
	: m_Scene(), m_ObjectList()
{
	m_Grid = std::make_unique<Grid>(m_Scene);
	m_MainCursor = std::make_unique<Cursor>(m_Scene);
}

SceneCAD::~SceneCAD()
{
	m_ObjectList.clear();
}

void SceneCAD::DeleteObject(SceneObjectCAD& object)
{
	auto id = object.Id();
	auto result = m_ObjectList.find(id);

	if (result == m_ObjectList.end())
	{
		return;
	}

	auto& objectPtr = result->second;

	objectPtr.reset();

	m_ObjectList.erase(id);
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateTorus(dxm::Vector3 position)
{
	auto torus = std::make_shared<Torus>(m_Scene, position);
	m_ObjectList.emplace(torus->Id(), torus);

	return torus;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreatePoint(dxm::Vector3 position)
{
	auto point = std::make_shared<Point>(m_Scene, position);
	m_ObjectList.emplace(point->Id(), point);

	return point;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateBezierCurveC0(std::vector<std::weak_ptr<Point>> controlPoints)
{
	auto curve = std::make_shared<BezierCurveC0>(m_Scene, controlPoints);
	m_ObjectList.emplace(curve->Id(), curve);

	for (auto& controlPoint : controlPoints)
	{
		auto controlPointShared = controlPoint.lock();
		controlPointShared->m_PointBasedObjects.push_back(curve);
	}

	return curve;
}
