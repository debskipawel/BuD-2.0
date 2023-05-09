#pragma once

#include "Scene.h"

#include <entt/entt.hpp>

#include <string>

namespace BuD
{
	class SceneEntity
	{
	public:
		SceneEntity() = default;
		SceneEntity(Scene& scene);
		SceneEntity(Scene& scene, entt::entity id);
		virtual ~SceneEntity() = default;

		uint32_t Id() const { return static_cast<uint32_t>(m_EntityID); }

		bool operator==(const SceneEntity& other) const
		{
			return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene;
		}

		bool operator!=(const SceneEntity& other) const
		{
			return !(*this == other);
		}

		Scene* operator->() { return m_Scene; }

	protected:
		entt::entity m_EntityID{ entt::null };
		Scene* m_Scene = nullptr;

	public:
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) const
		{
			return m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityID);;
		}

		template <typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registry.remove<T>(m_EntityID);
		}
	};
}
