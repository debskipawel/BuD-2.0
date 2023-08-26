#include "BaseIntersectionAlgorithm.h"

#include <Visitors/Validation/IntersectionEligibilityValidationVisitor.h>

#include <Visitors/Intersection/CalculatorPointOnSurface.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeU.h>
#include <Visitors/Intersection/CalculatorPartialDerivativeV.h>
#include <Visitors/Intersection/CalculatorNormalVector.h>

BaseIntersectionAlgorithm::BaseIntersectionAlgorithm(IntersectionAlgorithmParameters parameters)
	: m_Parameters(parameters)
{
}

PointMappingResult BaseIntersectionAlgorithm::MapWorldPointToSurface(dxm::Vector3 point, std::shared_ptr<SceneObjectCAD> surface)
{
	PointMappingResult result = {};

	auto minDistance = FLT_MAX;

	auto prevClosestPoint = dxm::Vector2{ 0.0f, 0.0f };
	auto closestPoint = dxm::Vector2{ 0.0f, 0.0f };
	auto divisionFactor = 1.0f;

	const int MAX_ITER = 15;
	int iterCount;

	for (iterCount = 0; iterCount < MAX_ITER; iterCount++)
	{
		divisionFactor *= 0.5f;
		prevClosestPoint = closestPoint;

		for (float u = 0.0f; u < 1.0f; u += divisionFactor)
		{
			for (float v = 0.0f; v < 1.0f; v += divisionFactor)
			{
				auto result = GetPoint(surface, u + 0.5f * divisionFactor, v + 0.5f * divisionFactor);
				auto length = (result - point).Length();

				if (length < minDistance)
				{
					minDistance = length;
					closestPoint = { u + 0.5f * divisionFactor, v + 0.5f * divisionFactor };
				}
			}
		}

		auto step = closestPoint - prevClosestPoint;

		if (divisionFactor < 1e-2f || step.Length() < 0.001f)
		{
			break;
		}
	}

	result.m_Parameter = closestPoint;
	result.m_ResultFound = iterCount < MAX_ITER;

	return result;
}

PointMappingResult BaseIntersectionAlgorithm::CloudMapWorldPointToSurface(dxm::Vector3 point, std::shared_ptr<SceneObjectCAD> surface)
{
	PointMappingResult result = {};

	float bestU = 0.0, bestV = 0.0;
	float bestDistance = FLT_MAX;

	float splitSize = 1.0f / 20;

	for (float uFirstLevel = 0.0; uFirstLevel <= 1.0; uFirstLevel += splitSize)
	{
		for (float vFirstLevel = 0.0; vFirstLevel <= 1.0; vFirstLevel += splitSize)
		{
			auto newPoint = GetPoint(surface, uFirstLevel, vFirstLevel);
			
			float bestSplitDist = dxm::Vector3::DistanceSquared(point, newPoint);
			float stepSize = splitSize / 4;

			float stepU = uFirstLevel;
			float stepV = vFirstLevel;

			while (stepSize > splitSize / 16)
			{
				float areaDistanceBest = DBL_MAX;
				float areaUBest = 0.0f;
				float areaVBest = 0.0f;

				for (int i = -1; i <= 1; i += 2)
				{
					for (int j = -1; j <= 1; j += 2)
					{
						float u = stepU + stepSize * i;
						float v = stepV + stepSize * j;
						
						u = std::clamp(u, 0.0f, 1.0f);
						v = std::clamp(v, 0.0f, 1.0f);

						auto surfacePosition = GetPoint(surface, u, v);
						auto distance = dxm::Vector3::DistanceSquared(point, surfacePosition);
						
						if (distance < bestDistance)
						{
							bestU = u;
							bestV = v;
							bestDistance = distance;
						}
					}
				}

				if (areaDistanceBest < bestSplitDist)
				{
					stepU = areaUBest;
					stepV = areaVBest;
					bestSplitDist = areaDistanceBest;
				}

				stepSize /= 2;
			}

			float u = std::clamp(stepU, 0.0f, 1.0f);
			float v = std::clamp(stepV, 0.0f, 1.0f);

			auto surfacePosition = GetPoint(surface, u, v);
			auto distance = dxm::Vector3::DistanceSquared(point, surfacePosition);

			if (distance < bestDistance)
			{
				bestU = u;
				bestV = v;
				bestDistance = distance;
			}
		}
	}

	result.m_ResultFound = true;
	result.m_Parameter = { bestU, bestV };

	return result;
}
