#pragma once

#include <BuD.h>

struct RobotArmConfiguration
{
public:
	RobotArmConfiguration(float L1, float L2);

	virtual auto UpdateConfiguration(const dxm::Vector2& p0, const dxm::Vector2& p2, float L1, float L2) -> void;

	virtual auto Valid() const -> bool;

	virtual auto GetP0() const -> dxm::Vector2;
	virtual auto GetP1() const -> dxm::Vector2;
	virtual auto GetP2() const -> dxm::Vector2;

	std::vector<dxm::Vector2> m_PointOptions;
	size_t m_PointOptionIndex;

protected:
	virtual auto SolveInverseKinematic(float L1, float L2) -> void;

	dxm::Vector2 m_P0, m_P2;
};
