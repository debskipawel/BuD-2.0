#include "bud_pch.h"
#include "Scene.h"

#include "SceneEntity.h"

#include <Camera/OrbitCamera.h>
#include <Objects/MeshLoader/MeshLoader.h>

namespace BuD
{
	Scene::Scene()
		: m_Registry()
	{
		m_ActiveCamera = std::make_shared<OrbitCamera>(1.0f, 1000.0f, 1.0f);
	}

	void Scene::DestroyEntity(SceneEntity entity)
	{
		if (m_Registry.valid(entity))
		{
			m_Registry.destroy(entity);
		}
	}
}
