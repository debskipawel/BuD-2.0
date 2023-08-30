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

IntersectionResult BaseIntersectionAlgorithm::Find()
{
	IntersectionResult result = {};

	auto startingPoint = StartingParameter();
	result.m_StartingPoint = FindInitialCommonPoint(startingPoint);

	if (!result.m_StartingPoint.m_ResultFound)
	{
		return result;
	}

	result.m_IntersectionFound = true;

	auto direction = GetInitialDirection(result.m_StartingPoint);

	auto forwardResult = FindAllCommonPointsInDirection(result.m_StartingPoint, direction, MarchingDirection::FORWARD);

	result.m_ForwardPoints = std::move(forwardResult.m_Points);
	result.m_LoopDetected = forwardResult.m_LoopDetected;

	if (!forwardResult.m_LoopDetected.has_value())
	{
		auto backwardsResult = FindAllCommonPointsInDirection(result.m_StartingPoint, direction, MarchingDirection::BACKWARD);
		result.m_BackwardsPoints = std::move(backwardsResult.m_Points);
	}

	return result;
}

StartingCommonPointResult BaseIntersectionAlgorithm::FindInitialCommonPoint(dxm::Vector4 startingPosition)
{
	for (auto& initialCommonPointFinder : m_NearestCommonPointFinders)
	{
		auto result = initialCommonPointFinder->FindNearestCommonPoint(startingPosition);

		if (result.m_ResultFound)
		{
			return result;
		}
	}

	StartingCommonPointResult result = {};
	result.m_ResultFound = false;

	return result;
}

NextCommonPointResult BaseIntersectionAlgorithm::FindNextPoint(const NextCommonPointResult& previousPoint, std::optional<dxm::Vector3> previousDirection, MarchingDirection marchingDirection)
{
	if (previousDirection.has_value())
	{
		previousDirection->Normalize();
	}

	auto result = m_MarchingCommonPointFinder->NextPoint(previousPoint.m_Parameter, m_Parameters.m_PointDistance, marchingDirection);

	if (!result.m_ResultFound && previousDirection.has_value())
	{
		result = m_MarchingCommonPointFinder->NextPoint(previousPoint.m_Parameter, previousDirection.value(), m_Parameters.m_PointDistance);
	}

	if (result.m_ResultFound)
	{
		return result;
	}

	NextCommonPointResult emptyResult = {};
	emptyResult.m_ResultFound = false;

	return emptyResult;
}

CommonPointSequenceResult BaseIntersectionAlgorithm::FindAllCommonPointsInDirection(StartingCommonPointResult startingCommonPoint, dxm::Vector3 direction, MarchingDirection marchingDirection)
{
	CommonPointSequenceResult result = {};

	NextCommonPointResult startingPoint = {};
	startingPoint.m_Parameter = startingCommonPoint.m_Parameter;
	startingPoint.m_Point = startingCommonPoint.m_Point;

	NextCommonPointResult previousPoint = startingPoint;

	std::optional<dxm::Vector3> previousDirection = std::nullopt;

	while (true)
	{
		auto nextPoint = m_MarchingCommonPointFinder->NextPoint(previousPoint.m_Parameter, m_Parameters.m_PointDistance, marchingDirection);

		if (!nextPoint.m_ResultFound && previousDirection.has_value())
		{
			previousDirection->Normalize();
			nextPoint = m_MarchingCommonPointFinder->NextPoint(previousPoint.m_Parameter, previousDirection.value(), m_Parameters.m_PointDistance);
		}

		if (nextPoint.m_ResultFound)
		{
			previousDirection = nextPoint.m_Point - previousPoint.m_Point;

			auto loopResult = m_LoopDetector->DetectLoop(startingPoint, previousPoint, nextPoint);

			if (loopResult.has_value())
			{
				result.m_LoopDetected = loopResult;
				return result;
			}

			result.m_Points.push_back(nextPoint);

			previousPoint = nextPoint;

			if (nextPoint.m_ShouldContinue)
			{
				continue;
			}
		}

		break;
	}

	return result;
}

PointMappingResult BaseIntersectionAlgorithm::MapWorldPointToSurface(dxm::Vector3 point, std::weak_ptr<SceneObjectCAD> surface)
{
	PointMappingResult result = {};

	float bestU = 0.0, bestV = 0.0;
	float bestDistance = FLT_MAX;

	float splitSize = 1.0f / 20;

	for (float uFirstLevel = 0.0; uFirstLevel <= 1.0; uFirstLevel += splitSize)
	{
		for (float vFirstLevel = 0.0; vFirstLevel <= 1.0; vFirstLevel += splitSize)
		{
			auto newPoint = m_Sampler->GetPoint(surface, uFirstLevel, vFirstLevel);
			
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

						auto surfacePosition = m_Sampler->GetPoint(surface, u, v);
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

			auto surfacePosition = m_Sampler->GetPoint(surface, u, v);
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
