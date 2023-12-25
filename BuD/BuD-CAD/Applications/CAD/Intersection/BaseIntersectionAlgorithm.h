#pragma once

#include <memory>

#include <Applications/CAD/Intersection/Structures/IntersectionStructures.h>
#include <Applications/CAD/Intersection/IntersectionAlgorithmParameters.h>
#include <Applications/CAD/Intersection/Sampler/ISampler.h>

#include <Applications/CAD/Intersection/InitialCommonPoint/AbstractInitialCommonPointFinder.h>
#include <Applications/CAD/Intersection/CommonPointMarching/AbstractCommonPointMarching.h>
#include <Applications/CAD/Intersection/LoopDetectors/AbstractLoopDetector.h>

#include <Applications/CAD/Scene/SceneObjectCAD.h>

class BaseIntersectionAlgorithm
{
public:
	explicit BaseIntersectionAlgorithm(IntersectionAlgorithmParameters parameters);

	virtual IntersectionResult Find();

protected:
	virtual dxm::Vector4 StartingParameter() = 0;
	virtual dxm::Vector3 GetInitialDirection(const StartingCommonPointResult& initialPosition) = 0;

	virtual StartingCommonPointResult FindInitialCommonPoint(dxm::Vector4 startingPosition);
	virtual std::optional<LoopResult> DetectLoop(const NextCommonPointResult& startingPoint, const NextCommonPointResult& previousPoint, const NextCommonPointResult& nextPoint);

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
	std::vector<std::unique_ptr<AbstractLoopDetector>> m_LoopDetectors;

	std::unique_ptr<AbstractCommonPointMarching> m_MarchingCommonPointFinder;
};
