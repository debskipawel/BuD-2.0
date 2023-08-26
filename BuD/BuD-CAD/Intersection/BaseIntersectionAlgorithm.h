#pragma once

#include <memory>

#include <Intersection/IntersectionStructures.h>
#include <Intersection/IntersectionAlgorithmParameters.h>
#include <Intersection/Sampler/ISampler.h>

#include <Scene/SceneObjectCAD.h>

class BaseIntersectionAlgorithm : public ISampler
{
public:
	explicit BaseIntersectionAlgorithm(IntersectionAlgorithmParameters parameters);

	virtual void Find() = 0;
	virtual const std::deque<dxm::Vector4>& Result() const { return m_IntersectionCurve; }

protected:
	std::deque<dxm::Vector4> m_IntersectionCurve;

	virtual PointMappingResult MapWorldPointToSurface(dxm::Vector3 point, std::shared_ptr<SceneObjectCAD> surface);
	virtual PointMappingResult CloudMapWorldPointToSurface(dxm::Vector3 point, std::shared_ptr<SceneObjectCAD> surface);

	IntersectionAlgorithmParameters m_Parameters;
};
