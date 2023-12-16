#include "MultiIntersectionAlgorithm.h"

#include <Applications/CAD/Intersection/InitialCommonPoint/SteepestDescentCommonPointFinder.h>
#include <Applications/CAD/Intersection/InitialCommonPoint/LinearApproximationCommonPointFinder.h>

#include <Applications/CAD/Intersection/CommonPointMarching/NewtonCommonPointMarching.h>
#include <Applications/CAD/Intersection/LoopDetectors/DotProductLoopDetector.h>
#include <Applications/CAD/Intersection/LoopDetectors/ShortestDistanceLoopDetector.h>
#include <Applications/CAD/Intersection/Sampler/VisitorSampler.h>

MultiIntersectionAlgorithm::MultiIntersectionAlgorithm(
	const IntersectionAlgorithmParameters& parameters, 
	std::weak_ptr<SceneObjectCAD> parameterizedObject1, 
	std::weak_ptr<SceneObjectCAD> parameterizedObject2
)
	: BaseIntersectionAlgorithm(parameters)
{
	m_ParameterizedObject1 = parameterizedObject1;
	m_ParameterizedObject2 = parameterizedObject2;

	m_NearestCommonPointFinders.emplace_back(std::make_unique<LinearApproximationCommonPointFinder>(m_ParameterizedObject1, m_ParameterizedObject2, parameters.m_Precision));
	m_NearestCommonPointFinders.emplace_back(std::make_unique<SteepestDescentInitialCommonPointFinder>(m_ParameterizedObject1, m_ParameterizedObject2, parameters.m_Precision));
	
	m_MarchingCommonPointFinder = std::make_unique<NewtonCommonPointMarching>(m_ParameterizedObject1, m_ParameterizedObject2, m_Parameters.m_Precision);
	
	//m_LoopDetectors.emplace_back(std::make_unique<DotProductLoopDetector>(m_Parameters.m_PointDistance));
	m_LoopDetectors.emplace_back(std::make_unique<ShortestDistanceLoopDetector>(m_ParameterizedObject1, m_ParameterizedObject2, m_Parameters.m_PointDistance));

	m_Sampler = std::make_unique<VisitorSampler>();
}

dxm::Vector4 MultiIntersectionAlgorithm::StartingParameter()
{
	if (m_Parameters.m_UseCursorAsStartingPoint)
	{
		auto cursorPosition = m_Parameters.m_CursorPosition;

		auto uv = MapWorldPointToSurface(cursorPosition, m_ParameterizedObject1);
		auto st = MapWorldPointToSurface(cursorPosition, m_ParameterizedObject2);

		if (!uv.m_ResultFound || !st.m_ResultFound)
		{
			return dxm::Vector4(NAN);
		}

		return { uv.m_Parameter.x, uv.m_Parameter.y, st.m_Parameter.x, st.m_Parameter.y };
	}

	constexpr auto surfaceDivisionCount = 16;
	constexpr auto surfaceSubPatchWidth = 1.0f / surfaceDivisionCount;

	constexpr auto minimumParameterDistance = 0.25f;
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

					auto P = m_Sampler->GetPoint(m_ParameterizedObject1, uv.x, uv.y);
					auto Q = m_Sampler->GetPoint(m_ParameterizedObject2, st.x, st.y);

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

dxm::Vector3 MultiIntersectionAlgorithm::GetInitialDirection(const StartingCommonPointResult& initialPosition)
{
	auto n1 = m_Sampler->GetNormal(m_ParameterizedObject1, initialPosition.m_Parameter.x, initialPosition.m_Parameter.y);
	auto n2 = m_Sampler->GetNormal(m_ParameterizedObject2, initialPosition.m_Parameter.z, initialPosition.m_Parameter.w);

	auto direction = n1.Cross(n2);
	direction.Normalize();

	return direction;
}

dxm::Vector4 MultiIntersectionAlgorithm::MapWorldPointToParameterSpace(dxm::Vector3 point)
{
	auto result1 = MapWorldPointToSurface(point, m_ParameterizedObject1);
	auto result2 = MapWorldPointToSurface(point, m_ParameterizedObject2);

	return dxm::Vector4(result1.m_Parameter.x, result1.m_Parameter.y, result2.m_Parameter.x, result2.m_Parameter.y);
}
