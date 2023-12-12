#include "RobotArmConfiguration.h"

#include <numbers>

RobotArmConfiguration::RobotArmConfiguration(float L1, float L2)
{
	UpdateConfiguration({ 0.0f, 0.0f }, { L1, L2 }, L1, L2);
}

RobotArmConfiguration::RobotArmConfiguration(float L1, float L2, float alpha, float gamma)
{
	m_P0 = { 0.0f, 0.0f };
	
	auto p1 = dxm::Vector2(L1 * cosf(alpha), L1 * sinf(alpha));

	m_PointOptions.push_back(p1);
	m_PointOptionIndex = 0;

	auto beta = gamma + alpha - std::numbers::pi_v<float>;

	m_P2 = p1 + L2 * dxm::Vector2(cosf(beta), sinf(beta));
}

auto RobotArmConfiguration::UpdateConfiguration(const dxm::Vector2& p0, const dxm::Vector2& p2, float L1, float L2) -> void
{
	m_P0 = p0;
	m_P2 = p2;

	SolveInverseKinematic(L1, L2);
}

auto RobotArmConfiguration::Valid() const -> bool
{
	return !m_PointOptions.empty();
}

auto RobotArmConfiguration::GetP0() const -> dxm::Vector2
{
	return m_P0;
}

auto RobotArmConfiguration::GetP1() const -> dxm::Vector2
{
	return m_PointOptions.empty() ? dxm::Vector2(NAN) : m_PointOptions[m_PointOptionIndex];
}

auto RobotArmConfiguration::GetP2() const -> dxm::Vector2
{
	return m_P2;
}

auto RobotArmConfiguration::GetL1() const -> float
{
	auto d1 = GetP1() - GetP0();
	
	return d1.Length();
}

auto RobotArmConfiguration::GetL2() const -> float
{
	auto d2 = GetP2() - GetP1();

	return d2.Length();
}

auto RobotArmConfiguration::ToAngleParameters(int width, int height) const -> std::pair<int, int>
{
	auto p0 = GetP0();
	auto p1 = GetP1();
	auto p2 = GetP2();

	auto d1 = p1 - p0;
	auto d2 = p2 - p1;

	auto alpha = atan2f(d1.y, d1.x);

	// mapping from [-pi, pi] to [0, 2pi]
	alpha = (alpha < 0.0f) ? alpha + 2.0f * std::numbers::pi_v<float> : alpha;

	auto beta = atan2f(d2.y, d2.x);

	// mapping from [-pi, pi] to [0, 2pi]
	beta = (beta < 0.0f) ? beta + 2.0f * std::numbers::pi_v<float> : beta;

	// in this case would be [-pi, 3pi], needs to be wrapped
	auto gamma = beta - alpha + std::numbers::pi_v<float>;

	// [0, 3pi]
	gamma = (gamma < 0.0f) ? gamma + 2.0f * std::numbers::pi_v<float> : gamma;
	// [0, 2pi]
	gamma = (gamma < 2.0f * std::numbers::pi_v<float>) ? gamma : gamma - 2.0f * std::numbers::pi_v<float>;

	auto y = static_cast<int>(roundf(0.5f * alpha * std::numbers::inv_pi_v<float> * static_cast<float>(height)));
	auto x = static_cast<int>(roundf(0.5f * gamma * std::numbers::inv_pi_v<float> * static_cast<float>(width)));

	return { x, y };
}

auto RobotArmConfiguration::SolveInverseKinematic(float L1, float L2) -> void
{
	auto p0 = dxm::Vector2(0.0f, 0.0f);
	auto p2 = m_P2 - m_P0;

	auto delta = -p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - (L1 - L2) * (L1 - L2)) * (p2.x * p2.x + p2.y * p2.y - (L1 + L2) * (L1 + L2));

	if (delta < 0.0f)
	{
		m_PointOptions = {};
		m_PointOptionIndex = 0;

		return;
	}

	if (delta < 1e-6f)
	{
		auto x = (p2.x * (L1 * L1 - L2 * L2 + p2.x * p2.x + p2.y * p2.y)) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
		auto y = (L1 * L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - L2 * L2)) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

		m_PointOptions = { { x + m_P0.x, y + m_P0.y } };
		m_PointOptionIndex = 0;

		return;
	}

	auto sqrtDelta = sqrtf(delta);

	auto x1 = (p2.x * (L1 * L1 - L2 * L2 + p2.x * p2.x + p2.y * p2.y) - sqrtDelta) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
	auto y1 = (L1 * L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - L2 * L2) + p2.x * sqrtDelta) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

	auto x2 = (p2.x * (L1 * L1 - L2 * L2 + p2.x * p2.x + p2.y * p2.y) + sqrtDelta) / (2.0f * (p2.x * p2.x + p2.y * p2.y));
	auto y2 = (L1 * L1 * p2.y * p2.y + p2.y * p2.y * (p2.x * p2.x + p2.y * p2.y - L2 * L2) - p2.x * sqrtDelta) / (2.0f * p2.y * (p2.x * p2.x + p2.y * p2.y));

	m_PointOptionIndex = m_PointOptions.size() > 1 ? m_PointOptionIndex : 0;

	m_PointOptions = { { x1 + m_P0.x, y1 + m_P0.y }, { x2 + m_P0.x, y2 + m_P0.y } };
}
