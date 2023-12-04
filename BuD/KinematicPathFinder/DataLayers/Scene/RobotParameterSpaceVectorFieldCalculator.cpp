#include "RobotParameterSpaceVectorFieldCalculator.h"

auto RobotParameterSpaceVectorFieldCalculator::CalculateVectorField(BuD::EditableTexture& parameterSpaceMap, std::pair<int, int> targetConfiguration) -> std::map<std::pair<int, int>, VectorFieldPoint>
{
	parameterSpaceMap.BeginEdit();

	auto width = static_cast<int>(parameterSpaceMap.Width());
	auto height = static_cast<int>(parameterSpaceMap.Height());

	auto hash = [width](int x, int y) { return width * y + x; };

	auto [xEnd, yEnd] = targetConfiguration;

	auto pointsQueue = std::queue<std::pair<int, int>>();
	auto distanceMap = std::map<std::pair<int, int>, VectorFieldPoint>();

	distanceMap[{ xEnd, yEnd }] = { 0U, { 0, 0 } };
	pointsQueue.emplace(xEnd, yEnd);

	while (!pointsQueue.empty())
	{
		auto [x, y] = pointsQueue.front();
		pointsQueue.pop();

		const auto& [distance, _] = distanceMap.at({ x, y });

		auto neighbours = std::array<std::pair<int, int>, 4> { std::make_pair(x, y + 1), std::make_pair(x - 1, y), std::make_pair(x, y - 1), std::make_pair(x + 1, y) };

		for (auto& [xn, yn] : neighbours)
		{
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

			auto color = parameterSpaceMap.Sample(xn, yn);

			if (color != dxm::Vector4(0.0f, 0.0f, 0.0f, 1.0f) || distanceMap.contains({ xn, yn }))
			{
				continue;
			}

			if (distanceMap.contains({ xn, yn }))
			{
				auto& [prevDistance, _] = distanceMap.at({ xn, yn });

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
