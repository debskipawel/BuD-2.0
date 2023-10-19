#pragma once

#include <Applications/CAD/Intersection/BaseIntersectionAlgorithm.h>

class SelfIntersectionAlgorithm : public BaseIntersectionAlgorithm
{
public:
	SelfIntersectionAlgorithm(IntersectionAlgorithmParameters parameters, std::weak_ptr<SceneObjectCAD> parameterizedObject);

protected:
	virtual dxm::Vector4 StartingParameter() override;
	virtual dxm::Vector3 GetInitialDirection(const StartingCommonPointResult& initialPosition) override;

	virtual StartingCommonPointResult FindInitialCommonPoint(dxm::Vector4 startingPosition) override;

	virtual dxm::Vector4 StartingParameterFromCursor();
	virtual dxm::Vector4 StartingParameterFromSubdivision();

	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject;
};
