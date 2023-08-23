#pragma once

#include <Intersection/BaseIntersectionAlgorithm.h>

class MultiIntersectionAlgorithm : public BaseIntersectionAlgorithm
{
public:
	MultiIntersectionAlgorithm(
		IntersectionAlgorithmParameters parameters,
		std::weak_ptr<SceneObjectCAD> parameterizedObject1,
		std::weak_ptr<SceneObjectCAD> parameterizedObject2
	);

	virtual void Find() override;

protected:
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject2;
};
