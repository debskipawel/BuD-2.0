#include "bud_pch.h"
#include "SceneEntity.h"

#include "Scene.h"

namespace BuD
{
	SceneEntity::SceneEntity(Scene& scene)
		: m_Scene(&scene)
	{
		m_EntityID = scene.m_Registry.create();
	}
	
	SceneEntity::SceneEntity(Scene& scene, entt::entity id)
		: m_EntityID(id), m_Scene(&scene)
	{
	}
}
