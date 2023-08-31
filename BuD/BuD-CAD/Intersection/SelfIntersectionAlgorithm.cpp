#include "SelfIntersectionAlgorithm.h"

#include <Intersection/InitialCommonPoint/LinearApproximationCommonPointFinder.h>
#include <Intersection/InitialCommonPoint/SteepestDescentCommonPointFinder.h>

#include <Intersection/CommonPointMarching/NewtonCommonPointMarching.h>
#include <Intersection/LoopDetectors/DotProductLoopDetector.h>
#include <Intersection/Sampler/VisitorSampler.h>

#include <Visitors/Intersection/CalculatorParameterShortestDistance.h>

SelfIntersectionAlgorithm::SelfIntersectionAlgorithm(IntersectionAlgorithmParameters parameters, std::weak_ptr<SceneObjectCAD> parameterizedObject)
	: BaseIntersectionAlgorithm(parameters)
{
	m_ParameterizedObject = parameterizedObject;

	m_NearestCommonPointFinders.emplace_back(std::make_unique<LinearApproximationCommonPointFinder>(m_ParameterizedObject, m_ParameterizedObject, parameters.m_Precision));
	m_NearestCommonPointFinders.emplace_back(std::make_unique<SteepestDescentInitialCommonPointFinder>(m_ParameterizedObject, m_ParameterizedObject, parameters.m_Precision));

	m_MarchingCommonPointFinder = std::make_unique<NewtonCommonPointMarching>(m_ParameterizedObject, m_ParameterizedObject, m_Parameters.m_Precision);
	m_LoopDetector = std::make_unique<DotProductLoopDetector>(m_Parameters.m_PointDistance);

	m_Sampler = std::make_unique<VisitorSampler>();
}

dxm::Vector4 SelfIntersectionAlgorithm::StartingParameter()
{
	return m_Parameters.m_UseCursorAsStartingPoint
		? StartingParameterFromCursor()
		: StartingParameterFromSubdivision();
}

dxm::Vector3 SelfIntersectionAlgorithm::GetInitialDirection(const StartingCommonPointResult& initialPosition)
{
	auto n1 = m_Sampler->GetNormal(m_ParameterizedObject, initialPosition.m_Parameter.x, initialPosition.m_Parameter.y);
	auto n2 = m_Sampler->GetNormal(m_ParameterizedObject, initialPosition.m_Parameter.z, initialPosition.m_Parameter.w);

	auto direction = n1.Cross(n2);
	direction.Normalize();

	return direction;
}

dxm::Vector4 SelfIntersectionAlgorithm::StartingParameterFromCursor()
{
	constexpr auto surfaceDivisionCount = 16;
	constexpr auto surfaceSubPatchWidth = 1.0f / surfaceDivisionCount;

	constexpr auto minimumParameterDistance = 0.25f;

	auto parameterDistanceCalculator = std::make_unique<CalculatorParameterShortestDistance>();

	auto cursorMappingResult = MapWorldPointToSurface(m_Parameters.m_CursorPosition, m_ParameterizedObject);
	auto uv = cursorMappingResult.m_Parameter;

	auto P = m_Sampler->GetPoint(m_ParameterizedObject, uv.x, uv.y);

	auto bestPoint = dxm::Vector4(uv.x, uv.y, 0.0f, 0.0f);
	auto bestPointDistance = FLT_MAX;

	for (auto i = 0; i < surfaceDivisionCount; i++)
	{
		for (auto j = 0; j < surfaceDivisionCount; j++)
		{
			auto st = dxm::Vector2{ (i + 0.5f) * surfaceSubPatchWidth, (j + 0.5f) * surfaceSubPatchWidth };

			parameterDistanceCalculator->SetParameters(uv, st);
			parameterDistanceCalculator->Visit(m_ParameterizedObject);

			auto diff = parameterDistanceCalculator->Result();

			if (diff.Length() < minimumParameterDistance)
			{
				continue;
			}

			auto Q = m_Sampler->GetPoint(m_ParameterizedObject, st.x, st.y);

			auto pointDiff = P - Q;
			auto pointDistance = pointDiff.Length();

			if (pointDistance < bestPointDistance)
			{
				bestPointDistance = pointDistance;
				bestPoint = dxm::Vector4{ uv.x, uv.y, st.x, st.y };
			}
		}
	}

	return bestPoint;
}

dxm::Vector4 SelfIntersectionAlgorithm::StartingParameterFromSubdivision()
{
	constexpr auto surfaceDivisionCount = 16;
	constexpr auto surfaceSubPatchWidth = 1.0f / surfaceDivisionCount;

	constexpr auto minimumParameterDistance = 0.25f;

	auto parameterDistanceCalculator = std::make_unique<CalculatorParameterShortestDistance>();

	auto bestPoint = dxm::Vector4(0.25f, 0.25f, 0.75f, 0.75f);
	auto bestPointDistance = FLT_MAX;

	for (auto i = 0; i < surfaceDivisionCount; i++)
	{
		for (auto j = 0; j < surfaceDivisionCount; j++)
		{
			auto uv = dxm::Vector2{ (i + 0.5f) * surfaceSubPatchWidth, (j + 0.5f) * surfaceSubPatchWidth };

			for (auto k = 0; k < surfaceDivisionCount; k++)
			{
				for (auto l = 0; l < surfaceDivisionCount; l++)
				{
					auto st = dxm::Vector2{ (k + 0.5f) * surfaceSubPatchWidth, (l + 0.5f) * surfaceSubPatchWidth };

					parameterDistanceCalculator->SetParameters(uv, st);
					parameterDistanceCalculator->Visit(m_ParameterizedObject);

					auto diff = parameterDistanceCalculator->Result();

					if (diff.Length() < minimumParameterDistance)
					{
						continue;
					}

					auto P = m_Sampler->GetPoint(m_ParameterizedObject, uv.x, uv.y);
					auto Q = m_Sampler->GetPoint(m_ParameterizedObject, st.x, st.y);

					auto pointDiff = P - Q;
					auto midPoint = 0.5f * (P + Q);

					auto pointDistance = pointDiff.Length();

					if (pointDistance < bestPointDistance)
					{
						bestPointDistance = pointDistance;
						bestPoint = dxm::Vector4{ uv.x, uv.y, st.x, st.y };
					}
				}
			}
		}
	}

	return bestPoint;
}
