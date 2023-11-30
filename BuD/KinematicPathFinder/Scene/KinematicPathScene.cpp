#include "KinematicPathScene.h"

KinematicPathScene::KinematicPathScene()
	: m_Scene(), m_Obstacles()
{
	m_Obstacles.emplace_back(std::make_shared<Obstacle>(m_Scene, dxm::Vector2(-0.5f, -0.2f), dxm::Vector2(0.2f, 0.5f), dxm::Vector3::UnitX));
}
