#pragma once

#include <BuD.h>

class RobotArm
{
public:
	RobotArm(BuD::Scene& scene, const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2, float depth, const dxm::Vector3& color);

	virtual auto UpdateRobotPoints(const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2) -> void;

protected:
	struct InstanceData
	{
		dxm::Vector3 m_Position;
		dxm::Vector2 m_Orientation;
		float m_Length;
		dxm::Vector3 m_Color;
	};

	std::array<InstanceData, 2> m_InstanceData;

	dxm::Vector2 m_P0, m_P1, m_P2;
	float m_Depth;

	BuD::SceneEntity m_RobotArmEntity;
};
