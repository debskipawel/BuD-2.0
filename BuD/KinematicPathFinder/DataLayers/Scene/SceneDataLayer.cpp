#include "SceneDataLayer.h"

#include <numbers>

#include <DataLayers/Scene/CollisionDetector.h>
#include <DataLayers/Scene/RobotParameterSpaceDrawer.h>
#include <DataLayers/Scene/RobotParameterSpaceVectorFieldCalculator.h>

constexpr auto START_L1 = 0.4f;
constexpr auto START_L2 = 0.25f;

SceneDataLayer::SceneDataLayer()
	: m_Scene(), m_ObstacleCollection(m_Scene),
	  m_Length1(START_L1), m_Length2(START_L2),
	  m_StartConfiguration(START_L1, START_L2), m_EndConfiguration(START_L1, START_L2),
	  m_AngleParameterMap(360, 360), m_AngleParameterMapWithPath(360, 360), m_ParameterSpacePathMap(360)
{
	auto P0 = dxm::Vector2::Zero;
	auto P1 = P0 + START_L1 * dxm::Vector2::UnitX;
	auto P2 = P1 + START_L2 * dxm::Vector2::UnitY;

	m_RobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.0f, dxm::Vector3::One);
	m_StartRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.25f * dxm::Vector3::One);
	m_EndRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.25f * dxm::Vector3::One);

	UpdateMeshes();
	RecalculateRobotPathsInParameterSpace();
	RedrawRobotAngleParameterSpace();
	RedrawPathInRobotAngleParameterSpace();
}

auto SceneDataLayer::AddNewObstacle() -> void
{
	m_ObstacleCollection.AcceptNewObstacle();

	UpdateStartConfigurationPoints(m_StartConfiguration.GetP0(), m_StartConfiguration.GetP2());
	UpdateEndConfigurationPoints(m_EndConfiguration.GetP0(), m_EndConfiguration.GetP2());

	RecalculateRobotPathsInParameterSpace();
	RedrawRobotAngleParameterSpace();
	RedrawPathInRobotAngleParameterSpace();
}

auto SceneDataLayer::UpdateStartConfigurationPoints(const dxm::Vector2& p0, const dxm::Vector2& p2) -> void
{
	m_StartConfiguration.UpdateConfiguration(p0, p2, m_Length1, m_Length2);

	auto& pointOptions = m_StartConfiguration.m_PointOptions;

	std::erase_if(pointOptions, [this](dxm::Vector2 p1)
		{
			return IsCollision(m_StartConfiguration.GetP0(), p1, m_StartConfiguration.GetP2());
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
	m_EndConfiguration.UpdateConfiguration(p0, p2, m_Length1, m_Length2);

	auto& pointOptions = m_EndConfiguration.m_PointOptions;

	std::erase_if(pointOptions, [this](dxm::Vector2 p1)
		{
			return IsCollision(m_EndConfiguration.GetP0(), p1, m_EndConfiguration.GetP2());
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
	m_Length1 = L1;
	m_Length2 = L2;

	UpdateStartConfigurationPoints(m_StartConfiguration.GetP0(), m_StartConfiguration.GetP2());

	UpdateEndConfigurationPoints(m_EndConfiguration.GetP0(), m_EndConfiguration.GetP2());

	RecalculateRobotPathsInParameterSpace();
	RedrawRobotAngleParameterSpace();
	RedrawPathInRobotAngleParameterSpace();
}

auto SceneDataLayer::UpdateMeshes() -> void
{
	auto start0 = m_StartConfiguration.GetP0();
	auto start1 = m_StartConfiguration.GetP1();
	auto start2 = m_StartConfiguration.GetP2();

	m_RobotArm->UpdateRobotPoints(start0, start1, start2);
	m_StartRobotArm->UpdateRobotPoints(start0, start1, start2);

	auto end0 = m_EndConfiguration.GetP0();
	auto end1 = m_EndConfiguration.GetP1();
	auto end2 = m_EndConfiguration.GetP2();

	m_EndRobotArm->UpdateRobotPoints(end0, end1, end2);
}

auto SceneDataLayer::UpdateMeshes(std::pair<float, float> currentConfiguration) -> void
{
	UpdateMeshes();

	auto gamma = currentConfiguration.first * std::numbers::pi_v<float> / 180.0f;
	auto alpha = currentConfiguration.second * std::numbers::pi_v<float> / 180.0f;
	auto beta = gamma + alpha - std::numbers::pi_v<float>;

	auto p0 = m_StartConfiguration.GetP0();
	auto p1 = p0 + dxm::Vector2(m_Length1 * cosf(alpha), m_Length1 * sinf(alpha));
	auto p2 = p1 + dxm::Vector2(m_Length2 * cosf(beta), m_Length2 * sinf(beta));

	m_RobotArm->UpdateRobotPoints(p0, p1, p2);
}

auto SceneDataLayer::FindPathFromStartingConfiguration() -> std::vector<std::pair<int, int>>
{
	auto path = std::vector<std::pair<int, int>>();

	if (!m_StartConfiguration.Valid())
	{
		return path;
	}

	auto startingPoint = m_StartConfiguration.ToAngleParameters(m_AngleParameterMap.Width(), m_AngleParameterMap.Height());

	if (!m_ParameterSpacePathMap.Contains(startingPoint))
	{
		return path;
	}

	auto point = startingPoint;

	while (true)
	{
		auto pathResult = m_ParameterSpacePathMap.Get(point);

		auto distance = pathResult.m_Distance;
		auto direction = pathResult.m_Direction;

		if (distance == 0)
		{
			break;
		}

		path.push_back(point);

		point = std::make_pair(point.first + direction.first, point.second + direction.second);
	} 

	return path;
}

auto SceneDataLayer::RedrawRobotAngleParameterSpace() -> void
{
	auto parameterSpaceDrawer = RobotParameterSpaceDrawer(m_ObstacleCollection);

	parameterSpaceDrawer.DrawRobotParameterSpace(
		m_StartConfiguration.GetP0(), 
		m_Length1, 
		m_Length2,
		m_AngleParameterMap,
		m_ParameterSpacePathMap
	);
}

auto SceneDataLayer::RedrawPathInRobotAngleParameterSpace() -> void
{
	m_AngleParameterMap.BeginEdit();
	m_AngleParameterMapWithPath.BeginEdit();

	m_AngleParameterMapWithPath.CopyFromBuffer(m_AngleParameterMap.GetRawBuffer());

	for (const auto& [x, y] : FindPathFromStartingConfiguration())
	{
		m_AngleParameterMapWithPath.PutPixel(x, y, dxm::Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	}

	m_AngleParameterMap.EndEdit();
	m_AngleParameterMapWithPath.EndEdit();
}

auto SceneDataLayer::RecalculateRobotPathsInParameterSpace() -> void
{
	if (!m_EndConfiguration.Valid())
	{
		return;
	}

	auto targetConfiguration = m_EndConfiguration.ToAngleParameters(m_AngleParameterMap.Width(), m_AngleParameterMap.Height());

	auto vectorFieldCalculator = RobotParameterSpaceVectorFieldCalculator(m_ObstacleCollection);
	m_ParameterSpacePathMap = vectorFieldCalculator.CalculateVectorField(m_AngleParameterMap, m_EndConfiguration);
}

auto SceneDataLayer::IsCollision(const dxm::Vector2& p0, const dxm::Vector2& p1, const dxm::Vector2& p2) -> bool
{
	auto& obstacleList = m_ObstacleCollection.GetAll();

	auto collisionDetector = CollisionDetector();

	for (auto& obstacle : obstacleList)
	{
		if (collisionDetector.IsColliding(*obstacle, p0, p1) || collisionDetector.IsColliding(*obstacle, p1, p2))
		{
			return true;
		}
	}

	return false;
}
