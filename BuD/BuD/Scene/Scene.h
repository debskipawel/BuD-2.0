#pragma once

#include <Camera/AbstractCamera.h>

#include <entt/entt.hpp>

#include <map>
#include <memory>
#include <filesystem>

namespace BuD
{
	class Scene
	{
	public:
		Scene();

		template<typename... Components>
		auto GetAllEntitiesWith() const
		{
			return m_Registry.view<Components...>().each();
		}

		std::shared_ptr<AbstractCamera> ActiveCamera() const { return m_ActiveCamera; }

	protected:
		std::shared_ptr<AbstractCamera> m_ActiveCamera;

		entt::registry m_Registry;

		friend class SceneEntity;

	public:
		void DestroyEntity(SceneEntity entity);
	};
}
