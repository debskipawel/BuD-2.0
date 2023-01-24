#pragma once

#include "SceneEntity.h"

#include <Camera/AbstractCamera.h>

#include <map>
#include <memory>
#include <filesystem>

namespace BuD
{
	class Scene
	{
	public:
		Scene();

		std::shared_ptr<AbstractCamera> ActiveCamera() const { return m_ActiveCamera; }

		std::shared_ptr<SceneEntity> CreateCube();

		std::map<uint64_t, std::shared_ptr<SceneEntity>> m_SceneEntities;

	protected:
		std::shared_ptr<AbstractCamera> m_ActiveCamera;
	};
}
