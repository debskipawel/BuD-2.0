#include "SceneDataLayer.h"

#include <numbers>

constexpr auto START_L1 = 0.4f;
constexpr auto START_L2 = 0.25f;

SceneDataLayer::SceneDataLayer()
	: m_Scene(), m_Obstacles(), m_StartConfiguration(START_L1, START_L2), m_EndConfiguration(START_L1, START_L2), m_AngleParameterMap(360, 360)
{
	auto P0 = dxm::Vector2::Zero;
	auto P1 = P0 + START_L1 * dxm::Vector2::UnitX;
	auto P2 = P1 + START_L2 * dxm::Vector2::UnitY;

	m_RobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.0f, dxm::Vector3::One);
	m_StartRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.4f * dxm::Vector3::One);
	m_EndRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.4f * dxm::Vector3::One);

	UpdateMeshes();
	UpdateAngleParameterMap();
}

auto SceneDataLayer::AddNewObstacle(std::shared_ptr<Obstacle> obstacle) -> void
{
	m_Obstacles.push_back(obstacle);

	UpdateStartConfigurationPoints(m_StartConfiguration.m_P0, m_StartConfiguration.m_P2);
	UpdateEndConfigurationPoints(m_EndConfiguration.m_P0, m_EndConfiguration.m_P2);

	UpdateAngleParameterMap();
}

auto SceneDataLayer::RemoveObstacle(size_t idx) -> void
{
	m_Obstacles.erase(m_Obstacles.begin() + idx);

	UpdateAngleParameterMap();
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

	if (m_StartConfiguration.m_PointOptionIndex >= pointOptions.size())
	{
		m_StartConfiguration.m_PointOptionIndex = 0;
	}

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

	if (m_EndConfiguration.m_PointOptionIndex >= pointOptions.size())
	{
		m_EndConfiguration.m_PointOptionIndex = 0;
	}

	UpdateMeshes();
}

auto SceneDataLayer::UpdateConfigurationLength(float L1, float L2) -> void
{
	m_StartConfiguration.m_L1 = L1;
	m_StartConfiguration.m_L2 = L2;
	
	UpdateStartConfigurationPoints(m_StartConfiguration.m_P0, m_StartConfiguration.m_P2);

	m_EndConfiguration.m_L1 = L1;
	m_EndConfiguration.m_L2 = L2;

	UpdateEndConfigurationPoints(m_EndConfiguration.m_P0, m_EndConfiguration.m_P2);

	UpdateAngleParameterMap();
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

auto SceneDataLayer::GetCollidingObstacle(const dxm::Vector2& p1, const dxm::Vector2& p2) -> std::shared_ptr<Obstacle>
{
	for (auto& obstacle : m_Obstacles)
	{
		if (IsCollision(p1, p2, *obstacle))
		{
			return obstacle;
		}
	}

	return std::shared_ptr<Obstacle>();
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

auto SceneDataLayer::UpdateAngleParameterMap() -> void
{
	m_AngleParameterMap.BeginEdit();

	auto width = m_AngleParameterMap.Width();
	auto height = m_AngleParameterMap.Height();

	auto L1 = m_StartConfiguration.GetLength1(), L2 = m_StartConfiguration.GetLength2();

	auto p0 = m_StartConfiguration.m_P0;

	m_AngleParameterMap.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

	for (int y = 0; y < height; y++)
	{
		auto alpha = 2.0f * static_cast<float>(y) / static_cast<float>(width) * std::numbers::pi_v<float>;

		auto p1 = p0 + dxm::Vector2(L1 * cosf(alpha), L1 * sinf(alpha));

		auto firstArmCollidingObstacle = GetCollidingObstacle(p0, p1);

		if (firstArmCollidingObstacle)
		{
			auto& color = firstArmCollidingObstacle->Color();

			for (int x = 0; x < width; x++)
			{
				m_AngleParameterMap.PutPixel(x, y, { color.x, color.y, color.z, 1.0f });
			}

			continue;
		}

		for (int x = 0; x < width; x++)
		{
			auto gamma = 2.0f * static_cast<float>(x) / static_cast<float>(height) * std::numbers::pi_v<float>;
			auto beta = gamma + alpha - std::numbers::pi_v<float>;

			auto p2 = p1 + dxm::Vector2(L2 * cosf(beta), L2 * sinf(beta));

			auto secondArmCollidingObstacle = GetCollidingObstacle(p1, p2);

			if (secondArmCollidingObstacle)
			{
				auto& color = secondArmCollidingObstacle->Color();
				m_AngleParameterMap.PutPixel(x, y, { color.x, color.y, color.z, 1.0f });
			}
		}
	}

	m_AngleParameterMap.EndEdit();
}
