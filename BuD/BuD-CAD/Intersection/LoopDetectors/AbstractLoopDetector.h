#pragma once

#include <Intersection/Structures/IntersectionStructures.h>

#include <Intersection/Sampler/ISampler.h>

class AbstractLoopDetector
{
public:
	virtual std::optional<LoopResult> DetectLoop(
		const NextCommonPointResult& starting, 
		const NextCommonPointResult& previous, 
		const NextCommonPointResult& current
	) = 0;

protected:
	std::unique_ptr<ISampler> m_Sampler;
};
