#include "SceneObjectCAD.h"

SceneObjectCAD::SceneObjectCAD(BuD::Scene& scene)
	: m_SceneEntity(scene)
{
}

SceneObjectCAD::~SceneObjectCAD()
{
	m_SceneEntity->DestroyEntity(m_SceneEntity);
}
