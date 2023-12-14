#include "ActiveEdgeTable.h"

auto ActiveEdgeTable::Add(const Edge& edge) -> void
{
	m_ActiveEdgeList.push_back(edge);
}

auto ActiveEdgeTable::CurrentUpperBound() -> std::optional<Edge>
{
	auto upperBoundIter = std::max_element(m_ActiveEdgeList.begin(), m_ActiveEdgeList.end(),
		[this](const Edge& e1, const Edge& e2)
		{
			auto v1 = e1.InterpolateV(m_U);
			auto v2 = e2.InterpolateV(m_U);

			if (v1 < v2)
			{
				return true;
			}

			if (v1 == v2)
			{
				auto d1 = e1.m_PlaneUV2.m_Parameter - e1.m_PlaneUV1.m_Parameter;
				auto d2 = e2.m_PlaneUV2.m_Parameter - e2.m_PlaneUV1.m_Parameter;

				return atan2f(d1.y, d1.x) < atan2(d2.y, d2.x);
			}

			return false;
		}
	);

	if (upperBoundIter == m_ActiveEdgeList.end())
	{
		return std::nullopt;
	}

	return *upperBoundIter;
}

auto ActiveEdgeTable::CurrentLowerBound() -> std::optional<Edge>
{
	auto lowerBoundIter = std::min_element(m_ActiveEdgeList.begin(), m_ActiveEdgeList.end(),
		[this](const Edge& e1, const Edge& e2)
		{
			auto v1 = e1.InterpolateV(m_U);
			auto v2 = e2.InterpolateV(m_U);

			if (v1 < v2)
			{
				return true;
			}

			if (v1 == v2)
			{
				auto d1 = e1.m_PlaneUV2.m_Parameter - e1.m_PlaneUV1.m_Parameter;
				auto d2 = e2.m_PlaneUV2.m_Parameter - e2.m_PlaneUV1.m_Parameter;

				return atan2f(d1.y, d1.x) < atan2(d2.y, d2.x);
			}

			return false;
		}
	);

	if (lowerBoundIter == m_ActiveEdgeList.end())
	{
		return std::nullopt;
	}

	return *lowerBoundIter;
}

auto ActiveEdgeTable::GetFirstIntersecting(const Edge& edge, float lowerBound, float upperBound) -> std::pair<std::optional<Edge>, float>
{
	constexpr auto NO_INTERSECTION = -1;

	auto closestIntersectionIdx = NO_INTERSECTION;
	auto closestIntersectionParam = 0.0f;

	for (size_t i = 0; i < m_ActiveEdgeList.size(); ++i)
	{
		const auto& activeEdge = m_ActiveEdgeList[i];

		auto t = Intersection(edge, activeEdge);

		if (!t.has_value())
		{
			continue;
		}

		auto u = (1.0f - t.value()) * edge.m_PlaneUV1.m_Parameter.x + t.value() * edge.m_PlaneUV2.m_Parameter.x;

		if (u < lowerBound || u > upperBound)
		{
			continue;
		}

		if (closestIntersectionIdx == NO_INTERSECTION || t.value() < closestIntersectionParam)
		{
			closestIntersectionIdx = i;
			closestIntersectionParam = t.value();
		}
	}

	if (closestIntersectionIdx == NO_INTERSECTION)
	{
		return { std::nullopt, closestIntersectionParam };
	}

	return { m_ActiveEdgeList[closestIntersectionIdx], closestIntersectionParam };
}

auto ActiveEdgeTable::Update(float u) -> void
{
	m_U = u;

	std::erase_if(m_ActiveEdgeList, [u](const Edge& e) { return e.m_PlaneUV2.m_Parameter.x <= u; });
}

auto ActiveEdgeTable::Intersection(const Edge& e1, const Edge& e2) -> std::optional<float>
{
	auto p1 = e1.m_PlaneUV1.m_Parameter;
	auto p2 = e2.m_PlaneUV1.m_Parameter;

	auto d1 = e1.m_PlaneUV2.m_Parameter - e1.m_PlaneUV1.m_Parameter;
	auto d2 = e2.m_PlaneUV2.m_Parameter - e2.m_PlaneUV1.m_Parameter;

	auto v = p2 - p1;

	auto det = d1.x * d2.y - d1.y * d2.x;

	if (fabsf(det) < 1e-4f)
	{
		return std::nullopt;
	}

	auto t1 = (v.x * d2.y - v.y * d2.x) / det;
	auto t2 = (v.x * d1.y - v.y * d1.x) / det;

	if (t1 < 0.0f || t1 > 1.0f || t2 < 0.0f || t2 > 1.0f)
	{
		return std::nullopt;
	}

	return t1;
}
