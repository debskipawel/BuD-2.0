#pragma once

#include "SceneEntity.h"

#include <Camera/AbstractCamera.h>

#include <map>
#include <memory>

namespace BuD
{
	class Scene
	{
	public:
		Scene();

		std::shared_ptr<AbstractCamera> ActiveCamera() const { return m_ActiveCamera; }

		std::map<UUID, std::shared_ptr<SceneEntity>> m_SceneEntities;

	protected:
		std::shared_ptr<AbstractCamera> m_ActiveCamera;
	};
}
