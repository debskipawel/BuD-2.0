#pragma once

#include <BuD.h>

struct RobotArmConfiguration
{
public:
	RobotArmConfiguration(float L1, float L2);

	virtual auto UpdateConfiguration(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void;

	constexpr inline virtual auto GetLength1() const -> size_t { return m_L1; }
	constexpr inline virtual auto GetLength2() const -> size_t { return m_L2; }

	std::vector<dxm::Vector2> m_PointOptions;
	size_t m_PointOptionIndex;

	dxm::Vector2 m_P0, m_P2;

	float m_L1, m_L2;

protected:
	virtual auto SolveInverseKinematic() -> void;
};
