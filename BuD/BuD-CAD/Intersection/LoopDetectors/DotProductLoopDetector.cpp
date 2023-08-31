#include "DotProductLoopDetector.h"

#include <Intersection/Sampler/VisitorSampler.h>

DotProductLoopDetector::DotProductLoopDetector(float pointDistance)
	: m_PointDistance(pointDistance)
{
	m_Sampler = std::make_unique<VisitorSampler>();
}

std::optional<LoopResult> DotProductLoopDetector::DetectLoop(const NextCommonPointResult& starting, const NextCommonPointResult& previous, const NextCommonPointResult& current)
{
	auto pointDiff = current.m_Point - starting.m_Point;

	if (pointDiff.Length() > m_PointDistance)
	{
		return std::nullopt;
	}

	auto initialStartToPrev = previous.m_Parameter - starting.m_Parameter;

	auto startToNext = current.m_Parameter - starting.m_Parameter;
	auto startToPrev = previous.m_Parameter - starting.m_Parameter;

	// handles special cases, when current point is on one side of parameter space, and the previous on the other
	// this logic took me like 2 hours to figure out and it works so please leave me alone and don't ask me why or how this works
	auto wrappingHandler = [](float initialStartToPrev, float wrapped, float& startToNext, float& startToPrev, float step)
	{
		auto determinant = initialStartToPrev * wrapped;

		if (determinant < 0.0f)
		{
			startToNext = startToPrev + step;

			return 0.0f;
		}
		
		startToPrev = startToNext - step;

		return wrapped;
	};

	auto loopWrapU = wrappingHandler(initialStartToPrev.x, current.m_WrappedU, startToNext.x, startToPrev.x, current.m_Step.x);
	auto loopWrapV = wrappingHandler(initialStartToPrev.y, current.m_WrappedV, startToNext.y, startToPrev.y, current.m_Step.y);
	auto loopWrapS = wrappingHandler(initialStartToPrev.z, current.m_WrappedS, startToNext.z, startToPrev.z, current.m_Step.z);
	auto loopWrapT = wrappingHandler(initialStartToPrev.w, current.m_WrappedT, startToNext.w, startToPrev.w, current.m_Step.w);

	auto startToNextUV = dxm::Vector2{ startToNext.x, startToNext.y };
	auto startToNextST = dxm::Vector2{ startToNext.z, startToNext.w };
	auto startToPrevUV = dxm::Vector2{ startToPrev.x, startToPrev.y };
	auto startToPrevST = dxm::Vector2{ startToPrev.z, startToPrev.w };

	if (startToNextUV.Dot(startToPrevUV) < 0.0f && startToNextST.Dot(startToPrevST) < 0.0f)
	{
		return LoopResult{ loopWrapU, loopWrapV, loopWrapS, loopWrapT };
	}

	return std::nullopt;
}
