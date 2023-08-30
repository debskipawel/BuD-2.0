#include "SelfIntersectionAlgorithm.h"

#include <Intersection/InitialCommonPoint/LinearApproximationCommonPointFinder.h>
#include <Intersection/InitialCommonPoint/SteepestDescentCommonPointFinder.h>

#include <Intersection/CommonPointMarching/NewtonCommonPointMarching.h>
#include <Intersection/LoopDetectors/DotProductLoopDetector.h>
#include <Intersection/Sampler/VisitorSampler.h>

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
	return dxm::Vector4(0.25f, 0.25f, 0.75f, 0.75f);
}

dxm::Vector3 SelfIntersectionAlgorithm::GetInitialDirection(const StartingCommonPointResult& initialPosition)
{
	auto n1 = m_Sampler->GetNormal(m_ParameterizedObject, initialPosition.m_Parameter.x, initialPosition.m_Parameter.y);
	auto n2 = m_Sampler->GetNormal(m_ParameterizedObject, initialPosition.m_Parameter.z, initialPosition.m_Parameter.w);

	auto direction = n1.Cross(n2);
	direction.Normalize();

	return direction;
}
