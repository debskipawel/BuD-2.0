#include "RobotParameterSpaceDrawer.h"

#include <numbers>

#include <DataLayers/Scene/CollisionDetector.h>

RobotParameterSpaceDrawer::RobotParameterSpaceDrawer(ObstacleCollection& obstacleCollection)
	: m_ObstacleCollection(obstacleCollection)
{
}

auto RobotParameterSpaceDrawer::DrawRobotParameterSpace(const dxm::Vector2& p0, float L1, float L2, BuD::EditableTexture& parameterSpaceMap) -> void
{
	parameterSpaceMap.BeginEdit();

	auto width = parameterSpaceMap.Width();
	auto height = parameterSpaceMap.Height();

	parameterSpaceMap.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

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
				parameterSpaceMap.PutPixel(x, y, { color.x, color.y, color.z, 1.0f });
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
				parameterSpaceMap.PutPixel(x, y, { color.x, color.y, color.z, 1.0f });
			}
		}
	}

	parameterSpaceMap.EndEdit();
}

auto RobotParameterSpaceDrawer::GetCollidingObstacle(const dxm::Vector2& p0, const dxm::Vector2& p1) -> std::shared_ptr<Obstacle>
{
	auto& obstacleList = m_ObstacleCollection.GetAll();

	auto collisionDetector = CollisionDetector();

	for (auto& obstacle : obstacleList)
	{
		if (collisionDetector.IsColliding(*obstacle, p0, p1))
		{
			return obstacle;
		}
	}

	return {};
}
