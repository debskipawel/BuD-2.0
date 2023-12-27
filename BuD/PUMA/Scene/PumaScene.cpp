#include "PumaScene.h"

PumaScene::PumaScene()
	: m_Scene()
{
	m_Grid = std::make_unique<Grid>(m_Scene);
}

auto PumaScene::Scene() -> BuD::Scene&
{
	return m_Scene;
}
