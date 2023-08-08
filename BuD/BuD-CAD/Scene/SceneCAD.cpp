#include "SceneCAD.h"

#include <Objects/Torus.h>
#include <Objects/Cube.h>

SceneCAD::SceneCAD()
	: m_Scene(), m_ObjectList()
{
}

std::shared_ptr<SceneObjectCAD> SceneCAD::CreateTorus(dxm::Vector3 position)
{
	auto torus = std::make_shared<Torus>(m_Scene, position);
	m_ObjectList.push_back(torus);

	return torus;
}
