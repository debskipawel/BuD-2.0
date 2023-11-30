#pragma once

#include <BuD.h>

class Obstacle
{
public:
	Obstacle(BuD::Scene& scene, const dxm::Vector2& min, const dxm::Vector2& max, const dxm::Vector3& color);

protected:
	struct ObstacleInstance
	{
		dxm::Vector3 m_Position;
		dxm::Vector2 m_Size;
		dxm::Vector3 m_Color;
	};

	ObstacleInstance m_InstanceData;

	BuD::SceneEntity m_RectEntity;
};
