#include "CrossSection.h"

#include <Applications/CAD/Intersection/MultiIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/ResultConverter/IntersectionResultConverter.h>
#include <Applications/CAD/Intersection/Sampler/VisitorSampler.h>

#include <Applications/CAD/Path/Generator/CrossSection/ActiveEdgeTable.h>

CrossSection::CrossSection(std::weak_ptr<SceneObjectCAD> plane, std::vector<std::shared_ptr<SceneObjectCAD>> model)
	: CrossSection(plane, model, { { 0.25f, 0.5f }, { 0.5f, 0.5f }, { 0.75f, 0.5f } })
{
}

CrossSection::CrossSection(std::weak_ptr<SceneObjectCAD> plane, std::vector<std::shared_ptr<SceneObjectCAD>> model, std::vector<dxm::Vector2> intersectionStartingParameters)
{
	auto startingPoints = std::vector<dxm::Vector3>();

	auto sampler = std::make_unique<VisitorSampler>();

	std::transform(intersectionStartingParameters.begin(), intersectionStartingParameters.end(), std::back_inserter(startingPoints),
		[plane, &sampler](const dxm::Vector2& parameter)
		{
			return sampler->GetPoint(plane, parameter.x, parameter.y);
		}
	);

	for (const auto& surface : model)
	{
		for (const auto& startingPoint : startingPoints)
		{
			auto intersectionParameters = IntersectionAlgorithmParameters();
			intersectionParameters.m_UseCursorAsStartingPoint = true;
			intersectionParameters.m_CursorPosition = startingPoint;

			auto intersectionCalculator = MultiIntersectionAlgorithm(intersectionParameters, plane, surface);

			auto intersectionResult = intersectionCalculator.Find();

			if (!intersectionResult.m_IntersectionFound)
			{
				return;
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

				m_IntersectionEdges.emplace_back(planePoints[index1], planePoints[index2], modelPoints[index1], modelPoints[index2], surface);
			}
		}
	}

	std::sort(m_IntersectionEdges.begin(), m_IntersectionEdges.end(),
		[](const Edge& e1, const Edge& e2)
		{
			return e1.m_PlaneUV1.m_Parameter.x < e2.m_PlaneUV1.m_Parameter.x;
		}
	);
}

auto CrossSection::UpperBound() -> std::vector<dxm::Vector2>
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
			u = (1.0f - t) * currentMaxEdge.StartU() + t * currentMaxEdge.EndU();;
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
	
	return result;
}

auto CrossSection::LowerBound() -> std::vector<dxm::Vector2>
{
	auto result = std::vector<dxm::Vector2>();

	return result;
}
