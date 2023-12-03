#include "RobotArmConfiguration.h"

RobotArmConfiguration::RobotArmConfiguration(float L1, float L2)
	: m_L1(L1), m_L2(L2)
{
	UpdateConfiguration({ 0.0f, 0.0f }, { L1, L2 });
}

auto RobotArmConfiguration::UpdateConfiguration(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void
{
	m_P0 = p0;
	m_P2 = p2;

	SolveInverseKinematic();
}

auto RobotArmConfiguration::SolveInverseKinematic() -> void
{
	auto p0 = dxm::Vector2(0.0f, 0.0f);
	auto p2 = m_P2 - m_P0;

	auto delta = -p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - (m_L1 - m_L2) * (m_L1 - m_L2)) * (p2.x * p2.x + p2.y * p2.y - (m_L1 + m_L2) * (m_L1 + m_L2));

	if (delta < 0.0f)
	{
		m_PointOptions = {};
		m_PointOptionIndex = 0;

		return;
	}

	if (delta < 1e-6f)
	{
		auto x = (p2.x * (m_L1 * m_L1 - m_L2 * m_L2 + p2.x * p2.x + p2.y * p2.y)) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
		auto y = (m_L1 * m_L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - m_L2 * m_L2)) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

		m_PointOptions = { { x + m_P0.x, y + m_P0.y } };
		m_PointOptionIndex = 0;

		return;
	}

	auto sqrtDelta = sqrtf(delta);

	auto x1 = (p2.x * (m_L1 * m_L1 - m_L2 * m_L2 + p2.x * p2.x + p2.y * p2.y) - sqrtDelta) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
	auto y1 = (m_L1 * m_L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - m_L2 * m_L2) + p2.x * sqrtDelta) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

	auto x2 = (p2.x * (m_L1 * m_L1 - m_L2 * m_L2 + p2.x * p2.x + p2.y * p2.y) + sqrtDelta) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
	auto y2 = (m_L1 * m_L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - m_L2 * m_L2) - p2.x * sqrtDelta) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

	m_PointOptionIndex = m_PointOptions.size() > 1 ? m_PointOptionIndex : 0;

	m_PointOptions = { { x1 + m_P0.x, y1 + m_P0.y }, { x2 + m_P0.x, y2 + m_P0.y } };
}
