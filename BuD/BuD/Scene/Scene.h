#pragma once

#include "SceneEntity.h"

#include <map>
#include <memory>

namespace BuD
{
	class Scene
	{
	public:
		Scene() = default;

		std::map<UUID, std::shared_ptr<SceneEntity>> m_SceneEntities;
	};
}
