#pragma once

#include <BuD.h>

#include "SceneObjectCAD.h"

#include <Objects/Additional/Cursor.h>
#include <Objects/Additional/Grid.h>
#include <Objects/CAD/Point.h>

class SceneCAD
{
public:
	SceneCAD();
	~SceneCAD();

	void DeleteObject(SceneObjectCAD& object);

	std::weak_ptr<SceneObjectCAD> CreateTorus(dxm::Vector3 position);
	std::weak_ptr<SceneObjectCAD> CreatePoint(dxm::Vector3 position);
	std::weak_ptr<SceneObjectCAD> CreateBezierCurveC0(std::vector<std::weak_ptr<Point>> controlPoints);

	std::shared_ptr<Cursor> m_MainCursor;

	std::map<uint32_t, std::shared_ptr<SceneObjectCAD>> m_ObjectList;

	BuD::Scene m_Scene;
	
protected:
	std::unique_ptr<Grid> m_Grid;
};
