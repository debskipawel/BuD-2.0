#pragma once

#include <BuD.h>

#include "SceneObjectCAD.h"
#include "CompositeSceneObject.h"

#include <Objects/Additional/Cursor.h>
#include <Objects/Additional/Grid.h>

class SceneCAD
{
public:
	SceneCAD();
	~SceneCAD();

	void DeleteObject(SceneObjectCAD& object);
	void DeleteSelected();

	std::shared_ptr<SceneObjectCAD> CreateTorus(dxm::Vector3 position);
	std::shared_ptr<SceneObjectCAD> CreatePoint(dxm::Vector3 position);

	std::shared_ptr<Cursor> m_MainCursor;

	std::map<uint32_t, std::shared_ptr<SceneObjectCAD>> m_ObjectList;
	CompositeSceneObject m_SelectedGroup;

	BuD::Scene m_Scene;
	
protected:
	std::unique_ptr<Grid> m_Grid;
};
