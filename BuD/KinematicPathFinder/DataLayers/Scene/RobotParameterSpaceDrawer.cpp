#include "RobotParameterSpaceDrawer.h"

#include <numbers>

#include <DataLayers/Scene/CollisionDetector.h>

RobotParameterSpaceDrawer::RobotParameterSpaceDrawer(ObstacleCollection& obstacleCollection)
	: m_ObstacleCollection(obstacleCollection)
{
}

auto RobotParameterSpaceDrawer::DrawRobotParameterSpace(const dxm::Vector2& p0, float L1, float L2, BuD::EditableTexture& parameterSpaceMap, const RobotParameterVectorField& vectorField) -> void
{
	parameterSpaceMap.BeginEdit();

	auto width = parameterSpaceMap.Width();
	auto height = parameterSpaceMap.Height();

	parameterSpaceMap.Clear({ 1.0f, 0.0f, 0.0f, 1.0f });

	//DrawObstacles(p0, L1, L2, parameterSpaceMap);

	DrawDistanceGradient(parameterSpaceMap, vectorField);

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

auto RobotParameterSpaceDrawer::DrawObstacles(const dxm::Vector2& p0, float L1, float L2, BuD::EditableTexture& parameterSpace) -> void
{
	auto width = parameterSpace.Width();
	auto height = parameterSpace.Height();

	parameterSpace.Clear({ 0.0f, 0.0f, 0.0f, 1.0f });

	for (int y = 0; y < height; y++)
	{
		auto alpha = 2.0f * static_cast<float>(y) / static_cast<float>(width) * std::numbers::pi_v<float>;

		auto p1 = p0 + dxm::Vector2(L1 * cosf(alpha), L1 * sinf(alpha));

		auto firstArmCollidingObstacle = GetCollidingObstacle(p0, p1);

		if (firstArmCollidingObstacle)
		{
			for (int x = 0; x < width; x++)
			{
				const auto& color = firstArmCollidingObstacle->Color();
				parameterSpace.PutPixel(x, y, { color.x, color.y, color.z, 1.0f });
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
				const auto& color = secondArmCollidingObstacle->Color();
				parameterSpace.PutPixel(x, y, { color.x, color.y, color.z, 1.0f });
			}
		}
	}
}

auto RobotParameterSpaceDrawer::DrawDistanceGradient(BuD::EditableTexture& parameterSpace, const RobotParameterVectorField& vectorField) -> void
{
	auto maxDistance = parameterSpace.Width() + parameterSpace.Height();

	vectorField.ForEach(
		[&parameterSpace, maxDistance](std::pair<int, int> pixel, VectorFieldPoint vectorFieldPoint)
		{
			auto distance = vectorFieldPoint.m_Distance;
			
			auto fract = 1.0f - static_cast<float>(distance) / static_cast<float>(maxDistance);
			fract = fract * fract;
			auto color = dxm::Vector4(fract, fract, fract, 1.0f);

			parameterSpace.PutPixel(pixel.first, pixel.second, color);
		}
	);
}
