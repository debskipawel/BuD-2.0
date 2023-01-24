#include "bud_pch.h"
#include "Scene.h"
#include "Cube.h"

#include <Camera/OrbitCamera.h>

namespace BuD
{
	Scene::Scene()
	{
		m_ActiveCamera = std::make_shared<OrbitCamera>(0.0f, 50.0f, 5.0f);
	}
	
	std::shared_ptr<SceneEntity> Scene::CreateCube()
	{
		auto cube = std::make_shared<Cube>(*this);
		m_SceneEntities.insert(std::make_pair(cube->Id(), cube));

		return cube;
	}
}
