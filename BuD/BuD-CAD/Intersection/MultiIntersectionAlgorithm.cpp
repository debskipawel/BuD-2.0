#include "MultiIntersectionAlgorithm.h"

#include <Intersection/InitialCommonPoint/SteepestDescentCommonPointFinder.h>
#include <Intersection/InitialCommonPoint/LinearApproximationCommonPointFinder.h>

#include <Intersection/CommonPointMarching/NewtonCommonPointMarching.h>

MultiIntersectionAlgorithm::MultiIntersectionAlgorithm(
	const IntersectionAlgorithmParameters& parameters, 
	std::weak_ptr<SceneObjectCAD> parameterizedObject1, 
	std::weak_ptr<SceneObjectCAD> parameterizedObject2
)
	: BaseIntersectionAlgorithm(parameters)
{
	m_ParameterizedObject1 = parameterizedObject1.lock();
	m_ParameterizedObject2 = parameterizedObject2.lock();

	m_NearestCommonPointFinders.emplace_back(std::make_unique<LinearApproximationCommonPointFinder>(m_ParameterizedObject1, m_ParameterizedObject2, parameters.m_Precision));
	m_NearestCommonPointFinders.emplace_back(std::make_unique<SteepestDescentInitialCommonPointFinder>(m_ParameterizedObject1, m_ParameterizedObject2, parameters.m_Precision));
	
	m_CommonPointMarching = std::make_unique<NewtonCommonPointMarching>(m_ParameterizedObject1, m_ParameterizedObject2, m_Parameters.m_Precision);
}

IntersectionResult MultiIntersectionAlgorithm::Find()
{
	IntersectionResult result = {};

	auto startingPoint = StartingParameter();
	result.m_StartingPoint = FindInitialCommonPoint(startingPoint);
	
	if (!result.m_StartingPoint.m_ResultFound)
	{
		return result;
	}

	result.m_IntersectionFound = true;

	auto n1 = GetNormal(m_ParameterizedObject1, result.m_StartingPoint.m_Parameter.x, result.m_StartingPoint.m_Parameter.y);
	auto n2 = GetNormal(m_ParameterizedObject2, result.m_StartingPoint.m_Parameter.z, result.m_StartingPoint.m_Parameter.w);

	auto direction = n1.Cross(n2);
	direction.Normalize();

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

dxm::Vector4 MultiIntersectionAlgorithm::StartingParameter()
{
	if (m_Parameters.m_UseCursorAsStartingPoint)
	{
		auto cursorPosition = m_Parameters.m_CursorPosition;

		auto uv = CloudMapWorldPointToSurface(cursorPosition, m_ParameterizedObject1);
		auto st = CloudMapWorldPointToSurface(cursorPosition, m_ParameterizedObject2);

		if (!uv.m_ResultFound || !st.m_ResultFound)
		{
			return dxm::Vector4(NAN);
		}

		return { uv.m_Parameter.x, uv.m_Parameter.y, st.m_Parameter.x, st.m_Parameter.y };
	}

	return dxm::Vector4{ 0.5f, 0.5f, 0.5f, 0.5f };
}

StartingCommonPointResult MultiIntersectionAlgorithm::FindInitialCommonPoint(dxm::Vector4 startingPosition)
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

CommonPointSequenceResult MultiIntersectionAlgorithm::FindAllCommonPointsInDirection(
	StartingCommonPointResult starting, 
	dxm::Vector3 direction, 
	MarchingDirection marchingDirection
)
{
	CommonPointSequenceResult result = {};

	NextCommonPointResult startingPoint = {};
	startingPoint.m_Parameter = starting.m_Parameter;
	startingPoint.m_Point = starting.m_Point;

	NextCommonPointResult previousPoint = startingPoint;

	std::optional<dxm::Vector3> previousDirection = std::nullopt;

	if (marchingDirection == MarchingDirection::BACKWARD)
	{
		direction = -direction;
	}

	while (true)
	{
		auto nextPoint = m_CommonPointMarching->NextPoint(previousPoint.m_Parameter, m_Parameters.m_PointDistance, marchingDirection);

		if (nextPoint.m_ResultFound)
		{
			previousDirection = nextPoint.m_Point - previousPoint.m_Point;

			auto loopResult = DetectLoop(startingPoint, previousPoint, nextPoint);

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

std::optional<LoopResult> MultiIntersectionAlgorithm::DetectLoop(NextCommonPointResult starting, NextCommonPointResult previous, NextCommonPointResult current)
{
	auto pointDiff = current.m_Point - starting.m_Point;
	
	if (pointDiff.Length() > m_Parameters.m_PointDistance)
	{
		return std::nullopt;
	}

	auto initialStartToPrev = previous.m_Parameter - starting.m_Parameter;

	auto startToNext = current.m_Parameter - starting.m_Parameter;
	auto startToPrev = previous.m_Parameter - starting.m_Parameter;

	// handles special cases, when current point is on one side of parameter space, and the previous on the other
	// this logic took me like 2 hours to figure out and it works so please leave me alone and don't ask me why or how this works
	auto wrappingHandler = [](float& initialStartToPrev, float& wrapped, float& startToNext, float& startToPrev, float& step)
	{
		auto determinant = initialStartToPrev * wrapped;

		if (determinant < 0.0f)
		{
			startToNext = startToPrev + step;
		}
		else if (determinant > 0.0f)
		{
			startToPrev = startToNext - step;
		}
	};

	wrappingHandler(initialStartToPrev.x, current.m_WrappedU, startToNext.x, startToPrev.x, current.m_Step.x);
	wrappingHandler(initialStartToPrev.y, current.m_WrappedV, startToNext.y, startToPrev.y, current.m_Step.y);
	wrappingHandler(initialStartToPrev.z, current.m_WrappedS, startToNext.z, startToPrev.z, current.m_Step.z);
	wrappingHandler(initialStartToPrev.w, current.m_WrappedT, startToNext.w, startToPrev.w, current.m_Step.w);

	auto startToNextUV = dxm::Vector2{ startToNext.x, startToNext.y };
	auto startToNextST = dxm::Vector2{ startToNext.z, startToNext.w };
	auto startToPrevUV = dxm::Vector2{ startToPrev.x, startToPrev.y };
	auto startToPrevST = dxm::Vector2{ startToPrev.z, startToPrev.w };

	if (startToNextUV.Dot(startToPrevUV) < 0.0f || startToNextST.Dot(startToPrevST) < 0.0f)
	{
		return LoopResult{ current.m_WrappedU, current.m_WrappedV, current.m_WrappedS, current.m_WrappedT };
	}

	return std::nullopt;
}

dxm::Vector4 MultiIntersectionAlgorithm::MapWorldPointToParameterSpace(dxm::Vector3 point)
{
	auto result1 = CloudMapWorldPointToSurface(point, m_ParameterizedObject1);
	auto result2 = CloudMapWorldPointToSurface(point, m_ParameterizedObject2);

	return dxm::Vector4(result1.m_Parameter.x, result1.m_Parameter.y, result2.m_Parameter.x, result2.m_Parameter.y);
}
