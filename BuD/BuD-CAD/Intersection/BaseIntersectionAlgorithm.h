#pragma once

#include <memory>

#include <Intersection/Structures/IntersectionStructures.h>
#include <Intersection/IntersectionAlgorithmParameters.h>
#include <Intersection/Sampler/ISampler.h>

#include <Scene/SceneObjectCAD.h>

class BaseIntersectionAlgorithm : public ISampler
{
public:
	explicit BaseIntersectionAlgorithm(IntersectionAlgorithmParameters parameters);

	virtual IntersectionResult Find() = 0;

protected:
	virtual PointMappingResult MapWorldPointToSurface(dxm::Vector3 point, std::shared_ptr<SceneObjectCAD> surface);
	virtual PointMappingResult CloudMapWorldPointToSurface(dxm::Vector3 point, std::shared_ptr<SceneObjectCAD> surface);

	IntersectionAlgorithmParameters m_Parameters;
};
