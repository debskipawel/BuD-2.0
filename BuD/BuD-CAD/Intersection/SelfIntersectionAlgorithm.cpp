#include "SelfIntersectionAlgorithm.h"

SelfIntersectionAlgorithm::SelfIntersectionAlgorithm(IntersectionAlgorithmParameters parameters, std::weak_ptr<SceneObjectCAD> parameterizedObject)
	: BaseIntersectionAlgorithm(parameters)
{
	m_ParameterizedObject = parameterizedObject.lock();
}

void SelfIntersectionAlgorithm::Find()
{
	// TODO: everything...
}
