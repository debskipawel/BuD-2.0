#include "bud_pch.h"
#include "Scene.h"

#include <Camera/OrbitCamera.h>

namespace BuD
{
	Scene::Scene()
	{
		m_ActiveCamera = std::make_shared<OrbitCamera>(0.0f, 50.0f, 5.0f);
	}
}
