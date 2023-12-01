#pragma once

#include <BuD.h>

class Obstacle
{
public:
	Obstacle(BuD::Scene& scene, const dxm::Vector2& v1, const dxm::Vector2& v2, const dxm::Vector3& color);

	virtual auto UpdateFirstVertex(const dxm::Vector2& v) -> void;
	virtual auto UpdateSecondVertex(const dxm::Vector2& v) -> void;

	virtual auto Color() -> dxm::Vector3&;

protected:
	virtual auto UpdateInstanceData() -> void;

	struct ObstacleInstance
	{
		dxm::Vector3 m_Position;
		dxm::Vector2 m_Size;
		dxm::Vector3 m_Color;
	};

	ObstacleInstance m_InstanceData;

	dxm::Vector2 m_Vertex1, m_Vertex2;

	BuD::SceneEntity m_RectEntity;
};
