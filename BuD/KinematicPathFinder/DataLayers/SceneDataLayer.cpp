#include "SceneDataLayer.h"

constexpr auto START_L1 = 0.4f;
constexpr auto START_L2 = 0.25f;

SceneDataLayer::SceneDataLayer()
	: m_Scene(), m_Obstacles(), m_StartConfiguration(START_L1, START_L2), m_EndConfiguration(START_L1, START_L2)
{
	auto P0 = dxm::Vector2::Zero;
	auto P1 = P0 + START_L1 * dxm::Vector2::UnitX;
	auto P2 = P1 + START_L2 * dxm::Vector2::UnitY;

	m_RobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.0f, dxm::Vector3::One);
	m_StartRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.4f * dxm::Vector3::One);
	m_EndRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.4f * dxm::Vector3::One);
}

auto SceneDataLayer::AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void
{
	m_Obstacles.push_back(obstacle);
}

auto SceneDataLayer::UpdateStartConfigurationPoints(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void
{
	m_StartConfiguration.UpdateConfiguration(p0, p2);

	auto& pointOptions = m_StartConfiguration.m_PointOptions;

	std::erase_if(pointOptions, [this](dxm::Vector2 p1)
		{
			return IsCollision(m_StartConfiguration.m_P0, p1, m_StartConfiguration.m_P2);
		}
	);

	UpdateMeshes();
}

auto SceneDataLayer::UpdateEndConfigurationPoints(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void
{
	m_EndConfiguration.UpdateConfiguration(p0, p2);

	auto& pointOptions = m_EndConfiguration.m_PointOptions;

	std::erase_if(pointOptions, [this](dxm::Vector2 p1)
		{
			return IsCollision(m_EndConfiguration.m_P0, p1, m_EndConfiguration.m_P2);
		}
	);

	UpdateMeshes();
}

auto SceneDataLayer::UpdateConfigurationLength(float L1, float L2) -> void
{
}

auto SceneDataLayer::UpdateMeshes() -> void
{
	auto start0 = m_StartConfiguration.m_P0;
	auto start1 = m_StartConfiguration.m_PointOptions.empty() ? dxm::Vector2(NAN) : m_StartConfiguration.m_PointOptions[m_StartConfiguration.m_PointOptionIndex];
	auto start2 = m_StartConfiguration.m_P2;

	m_StartRobotArm->UpdateRobotPoints(start0, start1, start2);

	auto end0 = m_EndConfiguration.m_P0;
	auto end1 = m_EndConfiguration.m_PointOptions.empty() ? dxm::Vector2(NAN) : m_EndConfiguration.m_PointOptions[m_EndConfiguration.m_PointOptionIndex];
	auto end2 = m_EndConfiguration.m_P2;

	m_EndRobotArm->UpdateRobotPoints(end0, end1, end2);
}

auto SceneDataLayer::IsCollision(const dxm::Vector2& p1, const dxm::Vector2& p2, const Obstacle& obstacle) -> bool
{
	auto o = p1;
	auto d = p2 - p1;

	auto position = obstacle.Position();
	auto size = obstacle.Size();

	std::array<dxm::Vector2, 4> positions = {};
	std::array<dxm::Vector2, 4> normals = {};
	std::array<float, 4> wallLengths = {};

	positions[0] = position + dxm::Vector2(0.0f, size.y * 0.5f);
	normals[0] = positions[0] - position;
	wallLengths[0] = size.x;

	positions[1] = position - dxm::Vector2(size.x * 0.5f, 0.0f);
	normals[1] = positions[1] - position;
	wallLengths[1] = size.y;

	positions[2] = position - dxm::Vector2(0.0f, size.y * 0.5f);
	normals[2] = positions[2] - position;
	wallLengths[2] = size.x;

	positions[3] = position + dxm::Vector2(size.x * 0.5f, 0.0f);
	normals[3] = positions[3] - position;
	wallLengths[3] = size.y;

	for (auto i = 0; i < positions.size(); ++i)
	{
		auto denominator1 = d.Dot(normals[i]);

		if (fabsf(denominator1) > 1e-4f)
		{
			auto numerator1 = normals[i].Dot(positions[i] - o);
			auto t = numerator1 / denominator1;

			if (t >= 0.0f && t <= 1.0f)
			{
				auto p = o + t * d;
				auto diff = positions[i] - p;
				auto maxDist = wallLengths[i] * 0.5f;

				if (diff.LengthSquared() < maxDist * maxDist)
				{
					return true;
				}
			}
		}
	}

	return false;
}

auto SceneDataLayer::IsCollision(const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2) -> bool
{
	for (auto& obstacle : m_Obstacles)
	{
		if (IsCollision(p0, p1, *obstacle) || IsCollision(p1, p2, *obstacle))
		{
			return true;
		}
	}

	return false;
}
