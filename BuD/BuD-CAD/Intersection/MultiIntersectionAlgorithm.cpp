#include "MultiIntersectionAlgorithm.h"

MultiIntersectionAlgorithm::MultiIntersectionAlgorithm(
	IntersectionAlgorithmParameters parameters, 
	std::weak_ptr<SceneObjectCAD> parameterizedObject1, 
	std::weak_ptr<SceneObjectCAD> parameterizedObject2
)
	: BaseIntersectionAlgorithm(parameters)
{
	m_ParameterizedObject1 = parameterizedObject1.lock();
	m_ParameterizedObject2 = parameterizedObject2.lock();
}

void MultiIntersectionAlgorithm::Find()
{
}
