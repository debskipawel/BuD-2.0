#include "bud_pch.h"
#include "Scene.h"

#include <Camera/OrbitCamera.h>
#include <Objects/Cube.h>

#include <Objects/MeshLoader/MeshLoader.h>

namespace BuD
{
	Scene::Scene()
		: m_Registry()
	{
		m_ActiveCamera = std::make_shared<OrbitCamera>(1.0f, 1000.0f, 1.0f);
	}
}
