#pragma once

#include <memory>

#include <Intersection/Structures/IntersectionStructures.h>
#include <Intersection/IntersectionAlgorithmParameters.h>
#include <Intersection/Sampler/ISampler.h>

#include <Intersection/InitialCommonPoint/AbstractInitialCommonPointFinder.h>
#include <Intersection/CommonPointMarching/AbstractCommonPointMarching.h>
#include <Intersection/LoopDetectors/AbstractLoopDetector.h>

#include <Scene/SceneObjectCAD.h>

class BaseIntersectionAlgorithm
{
public:
	explicit BaseIntersectionAlgorithm(IntersectionAlgorithmParameters parameters);

	virtual IntersectionResult Find();

protected:
	virtual dxm::Vector4 StartingParameter() = 0;
	virtual dxm::Vector3 GetInitialDirection(const StartingCommonPointResult& initialPosition) = 0;

	virtual StartingCommonPointResult FindInitialCommonPoint(dxm::Vector4 startingPosition);

	virtual NextCommonPointResult FindNextPoint(const NextCommonPointResult& previousPoint, std::optional<dxm::Vector3> previousStep, MarchingDirection marchingDirection);

	virtual CommonPointSequenceResult FindAllCommonPointsInDirection(
		StartingCommonPointResult startingCommonPoint,
		dxm::Vector3 direction,
		MarchingDirection marchingDirection
	);

	virtual PointMappingResult MapWorldPointToSurface(dxm::Vector3 point, std::weak_ptr<SceneObjectCAD> surface);

	IntersectionAlgorithmParameters m_Parameters;

	std::unique_ptr<ISampler> m_Sampler;

	std::vector<std::unique_ptr<AbstractInitialCommonPointFinder>> m_NearestCommonPointFinders;
	
	std::unique_ptr<AbstractCommonPointMarching> m_MarchingCommonPointFinder;
	std::unique_ptr<AbstractLoopDetector> m_LoopDetector;
};
