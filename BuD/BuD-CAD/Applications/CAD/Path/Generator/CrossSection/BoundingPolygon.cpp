#include "BoundingPolygon.h"

#include <algorithm>

BoundingPolygon::BoundingPolygon(const std::vector<dxm::Vector2>& polygonPoints, float defaultValue)
	: m_PolygonPoints(polygonPoints), m_DefaultValue(defaultValue)
{
	constexpr auto MIN_U = 0.0f;
	constexpr auto MAX_U = 1.0f;

	std::sort(m_PolygonPoints.begin(), m_PolygonPoints.end(),
		[](const dxm::Vector2& p1, const dxm::Vector2& p2)
		{
			return p1.x < p2.x;
		}
	);
}

auto BoundingPolygon::GetV(float u) const -> float
{
	auto it = std::lower_bound(m_PolygonPoints.begin(), m_PolygonPoints.end(), u,
		[](const dxm::Vector2& p, float u)
		{
			return p.x < u;
		}
	);

	if (it == m_PolygonPoints.end())
	{
		return m_DefaultValue;;
	}

	auto& uv = *it;

	if (u == uv.x)
	{
		return uv.y;
	}

	if (it == m_PolygonPoints.begin())
	{
		return m_DefaultValue;
	}

	auto prev = it - 1;

	return prev->y + (u - prev->x) / (it->x - prev->x) * (it->y - prev->y);
}

auto BoundingPolygon::GetPath(float u1, float u2) -> std::vector<dxm::Vector2>
{
	auto result = std::vector<dxm::Vector2>();

	result.emplace_back(u1, GetV(u1));
	
	auto it1 = std::lower_bound(m_PolygonPoints.begin(), m_PolygonPoints.end(), u1,
		[](const dxm::Vector2& p, float u)
		{
			return p.x < u;
		}
	);

	auto it2 = std::lower_bound(m_PolygonPoints.begin(), m_PolygonPoints.end(), u2,
		[](const dxm::Vector2& p, float u)
		{
			return p.x < u;
		}
	);

	for (auto i = it1; i < it2; i++)
	{
		result.emplace_back(i->x, i->y);
	}
	
	result.emplace_back(u2, GetV(u2));

	return result;
}

auto BoundingPolygon::Points() const -> const std::vector<dxm::Vector2>&
{
	return m_PolygonPoints;
}
