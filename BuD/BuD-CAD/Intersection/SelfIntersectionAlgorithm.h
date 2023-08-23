#pragma once

#include <Intersection/BaseIntersectionAlgorithm.h>

class SelfIntersectionAlgorithm : public BaseIntersectionAlgorithm
{
public:
	SelfIntersectionAlgorithm(IntersectionAlgorithmParameters parameters, std::weak_ptr<SceneObjectCAD> parameterizedObject);

	virtual void Find() override;

protected:
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject;
};
