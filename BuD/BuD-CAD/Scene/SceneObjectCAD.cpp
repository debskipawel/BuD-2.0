#include "SceneObjectCAD.h"

SceneObjectCAD::SceneObjectCAD(BuD::Scene& scene)
	: m_SceneEntity(scene), m_Selected(false)
{
}

SceneObjectCAD::~SceneObjectCAD()
{
	m_SceneEntity->DestroyEntity(m_SceneEntity);
}
