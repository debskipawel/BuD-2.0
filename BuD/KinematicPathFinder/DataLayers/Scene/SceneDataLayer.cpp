#include "SceneDataLayer.h"

#include <numbers>

#include <DataLayers/Scene/CollisionDetector.h>
#include <DataLayers/Scene/RobotParameterSpaceDrawer.h>
#include <DataLayers/Scene/RobotParameterSpaceVectorFieldCalculator.h>

constexpr auto START_L1 = 0.4f;
constexpr auto START_L2 = 0.25f;

SceneDataLayer::SceneDataLayer()
	: m_Scene(), m_Length1(START_L1), m_Length2(START_L2), m_ObstacleCollection(m_Scene), m_StartConfiguration(START_L1, START_L2), m_EndConfiguration(START_L1, START_L2), m_AngleParameterMap(360, 360)
{
	auto P0 = dxm::Vector2::Zero;
	auto P1 = P0 + START_L1 * dxm::Vector2::UnitX;
	auto P2 = P1 + START_L2 * dxm::Vector2::UnitY;

	m_RobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.0f, dxm::Vector3::One);
	m_StartRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.25f * dxm::Vector3::One);
	m_EndRobotArm = std::make_unique<RobotArm>(m_Scene, P0, P1, P2, 0.1f, 0.25f * dxm::Vector3::One);

	UpdateMeshes();
	RecalculateRobotAngleParameterSpace();
	RecalculateRobotPathsInParameterSpace();
}

auto SceneDataLayer::AddNewObstacle() -> void
{
	m_ObstacleCollection.AcceptNewObstacle();

	UpdateStartConfigurationPoints(m_StartConfiguration.GetP0(), m_StartConfiguration.GetP2());
	UpdateEndConfigurationPoints(m_EndConfiguration.GetP0(), m_EndConfiguration.GetP2());

	RecalculateRobotAngleParameterSpace();
	RecalculateRobotPathsInParameterSpace();
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
	UpdateStartConfigurationPoints(m_StartConfiguration.GetP0(), m_StartConfiguration.GetP2());

	UpdateEndConfigurationPoints(m_EndConfiguration.GetP0(), m_EndConfiguration.GetP2());

	RecalculateRobotAngleParameterSpace();
	RecalculateRobotPathsInParameterSpace();
}

auto SceneDataLayer::UpdateMeshes() -> void
{
	auto start0 = m_StartConfiguration.GetP0();
	auto start1 = m_StartConfiguration.GetP1();
	auto start2 = m_StartConfiguration.GetP2();

	m_StartRobotArm->UpdateRobotPoints(start0, start1, start2);

	auto end0 = m_EndConfiguration.GetP0();
	auto end1 = m_EndConfiguration.GetP1();
	auto end2 = m_EndConfiguration.GetP2();

	m_EndRobotArm->UpdateRobotPoints(end0, end1, end2);
}

auto SceneDataLayer::FindPathFromStartingConfiguration() -> std::vector<std::pair<int, int>>
{
	auto path = std::vector<std::pair<int, int>>();

	if (!m_StartConfiguration.Valid())
	{
		return path;
	}

	auto startingPoint = PointInParameterSpace(m_StartConfiguration);

	if (!m_ParameterSpacePathMap.contains(startingPoint))
	{
		return path;
	}

	auto point = startingPoint;

	while (true)
	{
		auto pathResult = m_ParameterSpacePathMap.at(point);

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

auto SceneDataLayer::RecalculateRobotAngleParameterSpace() -> void
{
	auto parameterSpaceDrawer = RobotParameterSpaceDrawer(m_ObstacleCollection);

	parameterSpaceDrawer.DrawRobotParameterSpace(
		m_StartConfiguration.GetP0(), 
		m_Length1, 
		m_Length2,
		m_AngleParameterMap
	);
}

auto SceneDataLayer::RecalculateRobotPathsInParameterSpace() -> void
{
	if (!m_EndConfiguration.Valid())
	{
		return;
	}

	auto targetConfiguration = PointInParameterSpace(m_EndConfiguration);

	auto vectorFieldCalculator = RobotParameterSpaceVectorFieldCalculator();
	m_ParameterSpacePathMap = vectorFieldCalculator.CalculateVectorField(m_AngleParameterMap, targetConfiguration);
}

auto SceneDataLayer::PointInParameterSpace(const RobotArmConfiguration& configuration) -> std::pair<int, int>
{
	auto p0 = configuration.GetP0();
	auto p1 = configuration.GetP1();
	auto p2 = configuration.GetP2();

	auto d1 = p1 - p0;
	auto d2 = p2 - p1;

	auto alpha = atan2f(d1.y, d1.x);
	alpha = (alpha < 0.0f) ? alpha + 2.0f * std::numbers::inv_pi_v<float> : alpha;
	
	auto beta = atan2f(d2.y, d2.x);
	beta = (beta < 0.0f) ? beta + + 2.0f * std::numbers::inv_pi_v<float> : beta;

	auto gamma = beta - alpha + std::numbers::pi_v<float>;

	auto y = static_cast<int>(roundf(0.5f * static_cast<float>(m_AngleParameterMap.Height()) * alpha * std::numbers::inv_pi_v<float>));
	auto x = static_cast<int>(roundf(0.5f * static_cast<float>(m_AngleParameterMap.Width())  * gamma * std::numbers::inv_pi_v<float>));

	return { x, y };
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
