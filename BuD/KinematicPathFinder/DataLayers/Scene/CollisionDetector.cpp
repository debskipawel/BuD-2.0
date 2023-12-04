#include "CollisionDetector.h"

auto CollisionDetector::IsColliding(const Obstacle& obstacle, const dxm::Vector2& p0, const dxm::Vector2& p1) -> bool
{
	auto o = p0;
	auto d = p1 - p0;

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
