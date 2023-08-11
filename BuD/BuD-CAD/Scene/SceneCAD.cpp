#include "SceneCAD.h"

#include <Objects/CAD/Torus.h>
#include <Objects/CAD/Point.h>

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

std::shared_ptr<SceneObjectCAD> SceneCAD::CreateTorus(dxm::Vector3 position)
{
	auto torus = std::make_shared<Torus>(m_Scene, position);
	m_ObjectList.emplace(torus->Id(), torus);

	return torus;
}

std::shared_ptr<SceneObjectCAD> SceneCAD::CreatePoint(dxm::Vector3 position)
{
	auto point = std::make_shared<Point>(m_Scene, position);
	m_ObjectList.emplace(point->Id(), point);

	return point;
}
