#pragma once

#include <BuD.h>

#include "SceneObjectCAD.h"
#include "CompositeSceneObject.h"

class SceneCAD
{
public:
	SceneCAD();

	std::shared_ptr<SceneObjectCAD> CreateTorus(dxm::Vector3 position);

	std::vector<std::shared_ptr<SceneObjectCAD>> m_ObjectList;
	CompositeSceneObject m_SelectedGroup;

	BuD::Scene m_Scene;
};
