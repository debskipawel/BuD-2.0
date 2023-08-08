#include "SceneCAD.h"

#include <Objects/Torus.h>
#include <Objects/Cube.h>

SceneCAD::SceneCAD()
	: m_Scene(), m_ObjectList()
{
	m_Grid = std::make_unique<Grid>(m_Scene);
}

std::shared_ptr<SceneObjectCAD> SceneCAD::CreateTorus(dxm::Vector3 position)
{
	auto torus = std::make_shared<Torus>(m_Scene, position);
	m_ObjectList.push_back(torus);

	return torus;
}
