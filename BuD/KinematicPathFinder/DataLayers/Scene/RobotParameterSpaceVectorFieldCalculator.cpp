#include "RobotParameterSpaceVectorFieldCalculator.h"

#include <numbers>

#include <DataLayers/Scene/CollisionDetector.h>

RobotParameterSpaceVectorFieldCalculator::RobotParameterSpaceVectorFieldCalculator(ObstacleCollection& obstacleCollection)
	: m_ObstacleCollection(obstacleCollection)
{
}

auto RobotParameterSpaceVectorFieldCalculator::CalculateVectorField(BuD::EditableTexture& parameterSpaceMap, const RobotArmConfiguration& targetConfiguration) -> RobotParameterVectorField
{
	parameterSpaceMap.BeginEdit();

	auto width = static_cast<int>(parameterSpaceMap.Width());
	auto height = static_cast<int>(parameterSpaceMap.Height());

	auto [xEnd, yEnd] = targetConfiguration.ToAngleParameters(width, height);

	auto L1 = targetConfiguration.GetL1();
	auto L2 = targetConfiguration.GetL2();

	auto pointsQueue = std::queue<std::pair<int, int>>();
	auto distanceMap = RobotParameterVectorField(width);

	distanceMap[{ xEnd, yEnd }] = { 0U, { 0, 0 } };
	pointsQueue.emplace(xEnd, yEnd);

	while (!pointsQueue.empty())
	{
		auto [x, y] = pointsQueue.front();

		pointsQueue.pop();

		const auto& [distance, _] = distanceMap.Get({ x, y });

		auto neighbours = GetNeighboursFloodFill8({ x, y }, width, height);

		for (auto& [xn, yn] : neighbours)
		{
			auto beta = 2.0f * std::numbers::pi_v<float> * static_cast<float>(xn) / static_cast<float>(width);
			auto alpha = 2.0f * std::numbers::pi_v<float> * static_cast<float>(yn) / static_cast<float>(height);
			
			auto configuration = RobotArmConfiguration(L1, L2, alpha, beta);

			auto collision = GetCollidingObject(configuration);

			if (collision)
			{
				continue;
			}

			if (distanceMap.Contains({ xn, yn }))
			{
				auto& [prevDistance, _] = distanceMap.Get({ xn, yn });

				if (prevDistance > distance + 1)
				{
					auto direction = std::make_pair(x - xn, y - yn);

					distanceMap[{ xn, yn }] = { distance + 1, direction };
				}

				continue;
			}

			auto direction = std::make_pair(x - xn, y - yn);

			distanceMap[{ xn, yn }] = { distance + 1, direction };
			pointsQueue.emplace(xn, yn);
		}
	}

	parameterSpaceMap.EndEdit();

	return distanceMap;
}

auto RobotParameterSpaceVectorFieldCalculator::WrapPixelParameters(std::pair<int, int> pixel, int width, int height) -> std::pair<int, int>
{
	auto& [xn, yn] = pixel;

	if (xn < 0)
	{
		xn += fabsf(floorf(static_cast<float>(xn) / width) * width);
	}
	else if (xn >= width)
	{
		xn -= (static_cast<float>(xn) / width) * width;
	}

	if (yn < 0)
	{
		yn += fabsf(floorf(static_cast<float>(yn) / height) * height);
	}
	else if (yn >= width)
	{
		yn -= (static_cast<float>(yn) / height) * height;
	}

	return pixel;
}

auto RobotParameterSpaceVectorFieldCalculator::GetNeighboursFloodFill4(std::pair<int, int> pixel, int width, int height)->std::array<std::pair<int, int>, 4>
{
	auto& [x, y] = pixel;

	return std::array<std::pair<int, int>, 4>
	{
		WrapPixelParameters(std::make_pair(x, y + 1), width, height),
		WrapPixelParameters(std::make_pair(x - 1, y), width, height),
		WrapPixelParameters(std::make_pair(x, y - 1), width, height),
		WrapPixelParameters(std::make_pair(x + 1, y), width, height),
	};
}

auto RobotParameterSpaceVectorFieldCalculator::GetNeighboursFloodFill8(std::pair<int, int> pixel, int width, int height) -> std::array<std::pair<int, int>, 8>
{
	auto& [x, y] = pixel;

	return std::array<std::pair<int, int>, 8>
	{
		WrapPixelParameters(std::make_pair(x, y + 1), width, height),
		WrapPixelParameters(std::make_pair(x - 1, y), width, height),
		WrapPixelParameters(std::make_pair(x, y - 1), width, height),
		WrapPixelParameters(std::make_pair(x + 1, y), width, height),
		WrapPixelParameters(std::make_pair(x - 1, y + 1), width, height),
		WrapPixelParameters(std::make_pair(x - 1, y + 1), width, height),
		WrapPixelParameters(std::make_pair(x + 1, y - 1), width, height),
		WrapPixelParameters(std::make_pair(x + 1, y + 1), width, height),
	};
}

auto RobotParameterSpaceVectorFieldCalculator::GetCollidingObject(const RobotArmConfiguration& configuration) -> std::shared_ptr<Obstacle>
{
	auto collisionDetector = CollisionDetector();

	for (const auto& obstacle : m_ObstacleCollection.GetAll())
	{
		if (collisionDetector.IsColliding(*obstacle, configuration.GetP0(), configuration.GetP1()) ||
			collisionDetector.IsColliding(*obstacle, configuration.GetP1(), configuration.GetP2()))
		{
			return obstacle;
		}
	}
}
