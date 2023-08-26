#pragma once

#include <Intersection/BaseIntersectionAlgorithm.h>
#include <Intersection/InitialCommonPoint/AbstractInitialCommonPointFinder.h>
#include <Intersection/CommonPointMarching/AbstractCommonPointMarching.h>

class MultiIntersectionAlgorithm : public BaseIntersectionAlgorithm
{
public:
	MultiIntersectionAlgorithm(
		const IntersectionAlgorithmParameters& parameters,
		std::weak_ptr<SceneObjectCAD> parameterizedObject1,
		std::weak_ptr<SceneObjectCAD> parameterizedObject2
	);

	virtual IntersectionResult Find() override;

protected:
	virtual dxm::Vector4 StartingParameter();
	virtual StartingCommonPointResult FindInitialCommonPoint(dxm::Vector4 startingPosition);

	virtual CommonPointSequenceResult FindAllCommonPointsInDirection(
		StartingCommonPointResult startingCommonPoint, 
		dxm::Vector3 direction, 
		MarchingDirection marchingDirection
	);

	virtual std::optional<LoopResult> DetectLoop(NextCommonPointResult starting, NextCommonPointResult previous, NextCommonPointResult current);

	virtual dxm::Vector4 MapWorldPointToParameterSpace(dxm::Vector3 point);

	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject2;

	std::vector<std::unique_ptr<AbstractInitialCommonPointFinder>> m_NearestCommonPointFinders;
	std::unique_ptr<AbstractCommonPointMarching> m_CommonPointMarching;
};
