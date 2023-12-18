#include "CrossSection.h"

#include <Applications/CAD/Intersection/MultiIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/ResultConverter/IntersectionResultConverter.h>
#include <Applications/CAD/Intersection/Sampler/VisitorSampler.h>

#include <Applications/CAD/Path/Generator/CrossSection/ActiveEdgeTable.h>

CrossSection::CrossSection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model)
{
	constexpr auto PLANE_SPLIT_U = 4U;
	constexpr auto PLANE_SPLIT_V = 2U;

	constexpr auto DU = 1.0f / (PLANE_SPLIT_U + 1);
	constexpr auto DV = 1.0f / (PLANE_SPLIT_V + 1);

	auto startingPoints = std::vector<dxm::Vector3>();
	auto sampler = std::make_unique<VisitorSampler>();

	for (size_t y = 0; y < PLANE_SPLIT_V; y++)
	{
		for (size_t x = 0; x < PLANE_SPLIT_U; x++)
		{
			auto u = (x + 1) * DU;
			auto v = (y + 1) * DV;

			auto point = sampler->GetPoint(plane, u, v);

			startingPoints.emplace_back(point);
		}
	}

	InitializeIntersectionEdgesCollection(plane, model, startingPoints);
}

CrossSection::CrossSection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model, const std::vector<dxm::Vector2>& intersectionStartingParameters)
{
	auto startingPoints = std::vector<dxm::Vector3>();

	auto sampler = std::make_unique<VisitorSampler>();

	std::transform(intersectionStartingParameters.begin(), intersectionStartingParameters.end(), std::back_inserter(startingPoints),
		[plane, &sampler](const dxm::Vector2& parameter)
		{
			return sampler->GetPoint(plane, parameter.x, parameter.y);
		}
	);

	InitializeIntersectionEdgesCollection(plane, model, startingPoints);
}

auto CrossSection::UpperBound() -> BoundingPolygon
{
	auto result = std::vector<dxm::Vector2>{ };
	
	if (m_IntersectionEdges.empty())
	{
		result.emplace_back(0.0f, 0.0f);
		result.emplace_back(1.0f, 0.0f);

		return result;
	}

	auto AET = ActiveEdgeTable();
	
	auto u = 0.0f;

	auto idx = 0;

	while (u < 1.0f)
	{
		auto currentMaxEdgeOpt = AET.CurrentUpperBound();

		if (!currentMaxEdgeOpt.has_value())
		{
			result.emplace_back(u, 0.0f);

			if (idx >= m_IntersectionEdges.size())
			{
				result.emplace_back(1.0f, 0.0f);

				break;
			}

			result.emplace_back(m_IntersectionEdges[idx].StartU(), 0.0f);

			u = m_IntersectionEdges[idx].StartU();

			while (idx < m_IntersectionEdges.size() && m_IntersectionEdges[idx].StartU() <= u)
			{
				AET.Add(m_IntersectionEdges[idx]);
				idx++;
			}

			AET.Update(u);

			continue;
		}

		const auto& currentMaxEdge = currentMaxEdgeOpt.value();

		result.emplace_back(u, currentMaxEdge.InterpolateV(u));

		auto intersectionLowerBound = u;
		auto intersectionUpperBound = idx < m_IntersectionEdges.size() ? min(currentMaxEdge.EndU(), m_IntersectionEdges[idx].StartU()) : currentMaxEdge.EndU();

		auto [intersectingEdge, t] = AET.GetFirstIntersecting(currentMaxEdge, intersectionLowerBound, intersectionUpperBound);

		if (intersectingEdge.has_value())
		{
			u = (1.0f - t) * currentMaxEdge.StartU() + t * currentMaxEdge.EndU();
			
			AET.Update(u);

			continue;
		}

		if (idx < m_IntersectionEdges.size())
		{
			const auto& nextEdge = m_IntersectionEdges[idx];

			auto gapBetweenEdges = nextEdge.StartU() > currentMaxEdge.EndU();

			if (gapBetweenEdges)
			{
				result.emplace_back(currentMaxEdge.EndU(), currentMaxEdge.EndV());

				u = currentMaxEdge.EndU();
			}
			else
			{
				auto t = (nextEdge.StartU() - currentMaxEdge.StartU()) / (currentMaxEdge.EndU() - currentMaxEdge.StartU());

				auto uIntersect = (1.0f - t) * currentMaxEdge.StartU() + t * currentMaxEdge.EndU();
				auto vIntersect = (1.0f - t) * currentMaxEdge.StartV() + t * currentMaxEdge.EndV();

				result.emplace_back(uIntersect, vIntersect);

				u = nextEdge.StartU();
			}
		}
		else
		{
			result.emplace_back(currentMaxEdge.EndU(), currentMaxEdge.EndV());

			u = currentMaxEdge.EndU();
		}

		while (idx < m_IntersectionEdges.size() && m_IntersectionEdges[idx].StartU() <= u)
		{
			AET.Add(m_IntersectionEdges[idx]);
			idx++;
		}

		AET.Update(u);
	}
	
	return BoundingPolygon(result);
}

auto CrossSection::LowerBound() -> BoundingPolygon
{
	auto result = std::vector<dxm::Vector2>{ };

	if (m_IntersectionEdges.empty())
	{
		result.emplace_back(0.0f, 1.0f);
		result.emplace_back(1.0f, 1.0f);

		return result;
	}

	auto AET = ActiveEdgeTable();

	auto u = 0.0f;

	auto idx = 0;

	while (u < 1.0f)
	{
		auto currentMaxEdgeOpt = AET.CurrentLowerBound();

		if (!currentMaxEdgeOpt.has_value())
		{
			result.emplace_back(u, 1.0f);

			if (idx >= m_IntersectionEdges.size())
			{
				result.emplace_back(1.0f, 1.0f);

				break;
			}

			result.emplace_back(m_IntersectionEdges[idx].StartU(), 1.0f);

			u = m_IntersectionEdges[idx].StartU();

			while (idx < m_IntersectionEdges.size() && m_IntersectionEdges[idx].StartU() <= u)
			{
				AET.Add(m_IntersectionEdges[idx]);
				idx++;
			}

			AET.Update(u);

			continue;
		}

		const auto& currentMaxEdge = currentMaxEdgeOpt.value();

		result.emplace_back(u, currentMaxEdge.InterpolateV(u));

		auto intersectionLowerBound = u;
		auto intersectionUpperBound = idx < m_IntersectionEdges.size() ? min(currentMaxEdge.EndU(), m_IntersectionEdges[idx].StartU()) : currentMaxEdge.EndU();

		auto [intersectingEdge, t] = AET.GetFirstIntersecting(currentMaxEdge, intersectionLowerBound, intersectionUpperBound);

		if (intersectingEdge.has_value())
		{
			u = (1.0f - t) * currentMaxEdge.StartU() + t * currentMaxEdge.EndU();

			AET.Update(u);

			continue;
		}

		if (idx < m_IntersectionEdges.size())
		{
			const auto& nextEdge = m_IntersectionEdges[idx];

			auto gapBetweenEdges = nextEdge.StartU() > currentMaxEdge.EndU();

			if (gapBetweenEdges)
			{
				result.emplace_back(currentMaxEdge.EndU(), currentMaxEdge.EndV());

				u = currentMaxEdge.EndU();
			}
			else
			{
				auto t = (nextEdge.StartU() - currentMaxEdge.StartU()) / (currentMaxEdge.EndU() - currentMaxEdge.StartU());

				auto uIntersect = (1.0f - t) * currentMaxEdge.StartU() + t * currentMaxEdge.EndU();
				auto vIntersect = (1.0f - t) * currentMaxEdge.StartV() + t * currentMaxEdge.EndV();

				result.emplace_back(uIntersect, vIntersect);

				u = nextEdge.StartU();
			}
		}
		else
		{
			result.emplace_back(currentMaxEdge.EndU(), currentMaxEdge.EndV());

			u = currentMaxEdge.EndU();
		}

		while (idx < m_IntersectionEdges.size() && m_IntersectionEdges[idx].StartU() <= u)
		{
			AET.Add(m_IntersectionEdges[idx]);
			idx++;
		}

		AET.Update(u);
	}

	return BoundingPolygon(result);
}

auto CrossSection::InitializeIntersectionEdgesCollection(std::weak_ptr<SceneObjectCAD> plane, const std::vector<std::shared_ptr<SceneObjectCAD>>& model, const std::vector<dxm::Vector3>& intersectionStartingPoints) -> void
{
	for (const auto& surface : model)
	{
		auto surfaceIntersectionEdges = std::vector<Edge>();

		for (const auto& startingPoint : intersectionStartingPoints)
		{
			auto intersectionParameters = IntersectionAlgorithmParameters();
			intersectionParameters.m_UseCursorAsStartingPoint = true;
			intersectionParameters.m_CursorPosition = startingPoint;

			auto intersectionCalculator = MultiIntersectionAlgorithm(intersectionParameters, plane, surface);

			auto intersectionResult = intersectionCalculator.Find();

			if (!intersectionResult.m_IntersectionFound)
			{
				continue;
			}

			auto converter = IntersectionResultConverter();

			auto [planePoints, modelPoints] = converter.ToPointVectors(intersectionResult);

			for (size_t i = 0; i + 1 < planePoints.size(); ++i)
			{
				auto index1 = i, index2 = i + 1;

				if (planePoints[index1].m_Parameter.x > planePoints[index2].m_Parameter.x)
				{
					index1 = i + 1;
					index2 = i;
				}

				if (planePoints[index1].m_Parameter.x == planePoints[index2].m_Parameter.x)
				{
					continue;
				}

				surfaceIntersectionEdges.emplace_back(planePoints[index1], planePoints[index2], modelPoints[index1], modelPoints[index2], surface);
			}
		}

		ConnectDiscontinuousPath(surface, surfaceIntersectionEdges);

		for (const auto& edge : surfaceIntersectionEdges)
		{
			m_IntersectionEdges.push_back(edge);
		}
	}

	std::sort(m_IntersectionEdges.begin(), m_IntersectionEdges.end(),
		[](const Edge& e1, const Edge& e2)
		{
			return e1.m_PlaneUV1.m_Parameter.x < e2.m_PlaneUV1.m_Parameter.x;
		}
	);
}

auto CrossSection::ConnectDiscontinuousPath(const std::shared_ptr<SceneObjectCAD>& object, std::vector<Edge>& intersectionEdges) -> void
{
	if (intersectionEdges.empty())
	{
		return;
	}

	auto offsetSurface = std::dynamic_pointer_cast<OffsetSurface>(object);
	
	if (!offsetSurface || !std::dynamic_pointer_cast<BezierSurfaceC0>(offsetSurface->InternalSurface().lock()))
	{
		return;
	}

	std::sort(intersectionEdges.begin(), intersectionEdges.end(),
		[](const Edge& e1, const Edge& e2)
		{
			return e1.m_PlaneUV1.m_Parameter.x < e2.m_PlaneUV1.m_Parameter.x;
		}
	);

	auto firstEdge = intersectionEdges.begin();
	auto lastEdge = std::max_element(intersectionEdges.begin(), intersectionEdges.end(),
		[](const Edge& e1, const Edge& e2)
		{
			return e1.m_PlaneUV2.m_Parameter.x < e2.m_PlaneUV2.m_Parameter.x;
		}
	);

	auto minV = min(firstEdge->StartV(), lastEdge->EndV());

	std::erase_if(intersectionEdges, [minV](const Edge& e) { return e.StartV() < minV && e.EndV() < minV; });

	for (size_t i = 0; i + 1 < intersectionEdges.size(); ++i)
	{
		const auto& e1 = intersectionEdges[i];
		const auto& e2 = intersectionEdges[i + 1];

		auto direction = dxm::Vector2(e2.StartU() - e1.EndU(), e2.StartV() - e1.EndV());

		if (direction.x > 0.0f)
		{
			auto newEdge = Edge(e1.m_PlaneUV2, e2.m_PlaneUV1, e1.m_ModelUV2, e2.m_ModelUV1, object);
			
			intersectionEdges.insert(intersectionEdges.begin() + i + 1, { newEdge });
		}
	}
}
