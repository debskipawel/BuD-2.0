#pragma once

#include <Utils/UUID.h>

#include <string>

namespace BuD
{
	class Scene;

	class SceneEntity
	{
	public:
		virtual ~SceneEntity() = default;

		uint64_t Id() const { return m_Id; }

		template <typename T>
		bool Implements() const { return dynamic_cast<const T*>(this); }

		template <typename T>
		const T* Component() { return dynamic_cast<const T*>(this); }

	protected:
		SceneEntity(Scene& scene);

		Scene* m_Scene;
		UUID m_Id;

	public:
		std::string m_Tag;
	};
}
