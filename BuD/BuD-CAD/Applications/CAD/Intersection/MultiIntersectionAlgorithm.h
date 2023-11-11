#pragma once

#include <Applications/CAD/Intersection/BaseIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/InitialCommonPoint/AbstractInitialCommonPointFinder.h>
#include <Applications/CAD/Intersection/CommonPointMarching/AbstractCommonPointMarching.h>

class MultiIntersectionAlgorithm : public BaseIntersectionAlgorithm
{
public:
	MultiIntersectionAlgorithm(
		const IntersectionAlgorithmParameters& parameters,
		std::weak_ptr<SceneObjectCAD> parameterizedObject1,
		std::weak_ptr<SceneObjectCAD> parameterizedObject2
	);

protected:
	virtual dxm::Vector4 StartingParameter() override;
	virtual dxm::Vector3 GetInitialDirection(const StartingCommonPointResult& initialPosition) override;

	virtual dxm::Vector4 MapWorldPointToParameterSpace(dxm::Vector3 point);

	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject2;
};
