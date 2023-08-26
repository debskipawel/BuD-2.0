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

void MultiIntersectionAlgorithm::Find()
{
	auto startingPoint = StartingPoint();
	auto commonStartingPoint = FindInitialCommonPoint(startingPoint);
	
	if (!commonStartingPoint.m_ResultFound)
	{
		m_IntersectionCurve = {};
		return;
	}

	m_IntersectionCurve = { commonStartingPoint.m_Parameter };

	auto n1 = GetNormal(m_ParameterizedObject1, commonStartingPoint.m_Parameter.x, commonStartingPoint.m_Parameter.y);
	auto n2 = GetNormal(m_ParameterizedObject2, commonStartingPoint.m_Parameter.z, commonStartingPoint.m_Parameter.w);

	auto direction = n1.Cross(n2);
	direction.Normalize();

	auto result = FindAllCommonPointsInDirection(commonStartingPoint, direction, MarchingDirection::FORWARD);
	
	if (!result.m_LoopDetected)
	{
		FindAllCommonPointsInDirection(commonStartingPoint, direction, MarchingDirection::BACKWARD);
	}
}

dxm::Vector4 MultiIntersectionAlgorithm::StartingPoint()
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

	std::optional<dxm::Vector3> previousDirection = std::nullopt;

	dxm::Vector2 startingUV = { starting.m_Parameter.x, starting.m_Parameter.y };
	dxm::Vector2 startingST = { starting.m_Parameter.z, starting.m_Parameter.w };

	NextCommonPointResult previousPoint = {};
	previousPoint.m_Parameter = starting.m_Parameter;
	previousPoint.m_Point = starting.m_Point;

	if (marchingDirection == MarchingDirection::BACKWARD)
	{
		direction = -direction;
	}

	constexpr auto MAX_ITER = 1000;
	auto iter = 0;

	while (true)
	{
		auto nextCommonPoint = m_CommonPointMarching->NextPoint(previousPoint.m_Parameter, m_Parameters.m_PointDistance, marchingDirection);

		// if we encounter parallel normals along our path, just continue in the same direction as before and hope for the best
		//if (!nextCommonPoint.m_ResultFound && previousDirection.has_value())
		//{
		//	previousDirection.value().Normalize();

		//	nextCommonPoint = m_CommonPointMarching->NextPoint(previousPoint.m_Parameter, previousDirection.value(), m_Parameters.m_PointDistance);
		//}

		if (nextCommonPoint.m_ResultFound)
		{
			previousDirection = nextCommonPoint.m_Point - previousPoint.m_Point;

			auto pointDiff = nextCommonPoint.m_Point - starting.m_Point;

			auto prevUV = dxm::Vector2{ previousPoint.m_Parameter.x, previousPoint.m_Parameter.y };
			auto prevST = dxm::Vector2{ previousPoint.m_Parameter.z, previousPoint.m_Parameter.w };

			auto UV = dxm::Vector2{ nextCommonPoint.m_Parameter.x, nextCommonPoint.m_Parameter.y };
			auto ST = dxm::Vector2{ nextCommonPoint.m_Parameter.z, nextCommonPoint.m_Parameter.w };

			auto prevStepUV = prevUV - startingUV;
			auto stepUV = UV - startingUV;

			auto prevStepST = prevST - startingST;
			auto stepST = ST - startingST;

			if (prevStepUV.Dot(stepUV) < 0.0f || prevStepST.Dot(stepST) < 0.0f)
			{
				int a = 5;
			}

			if (pointDiff.Length() <= m_Parameters.m_PointDistance && (prevStepUV.Dot(stepUV) < 0.0f || prevStepST.Dot(stepST) < 0.0f))
			{
				result.m_LoopDetected = true;
				return result;
			}

			if (marchingDirection == MarchingDirection::BACKWARD)
			{
				m_IntersectionCurve.push_front(nextCommonPoint.m_Parameter);
			}
			else
			{
				m_IntersectionCurve.push_back(nextCommonPoint.m_Parameter);
			}

			previousPoint = nextCommonPoint;
		}

		if (!nextCommonPoint.m_ResultFound || !nextCommonPoint.m_ShouldContinue)
		{
			break;
		}
	}

	return result;
}

dxm::Vector4 MultiIntersectionAlgorithm::MapWorldPointToParameterSpace(dxm::Vector3 point)
{
	auto result1 = CloudMapWorldPointToSurface(point, m_ParameterizedObject1);
	auto result2 = CloudMapWorldPointToSurface(point, m_ParameterizedObject2);

	return dxm::Vector4(result1.m_Parameter.x, result1.m_Parameter.y, result2.m_Parameter.x, result2.m_Parameter.y);
}
