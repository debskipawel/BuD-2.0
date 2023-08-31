#pragma once

#include <Intersection/LoopDetectors/AbstractLoopDetector.h>

class DotProductLoopDetector : public AbstractLoopDetector
{
public:
	DotProductLoopDetector(float pointDistance);

	virtual std::optional<LoopResult> DetectLoop(
		const NextCommonPointResult& starting,
		const NextCommonPointResult& previous,
		const NextCommonPointResult& current
	) override;

protected:
	float m_PointDistance;
};
