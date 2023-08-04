#pragma once

#include <BuD.h>

#include "SceneObjectCAD.h"

class SceneCAD
{
public:
	SceneCAD();

	std::shared_ptr<SceneObjectCAD> CreateTorus(dxm::Vector3 position);

	std::vector<std::shared_ptr<SceneObjectCAD>> m_ObjectList;
	BuD::Scene m_Scene;
};
