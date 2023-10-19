#pragma once

#include <deque>
#include <optional>

#include <Applications/CAD/Intersection/Structures/LoopResult.h>
#include <Applications/CAD/Intersection/Structures/StartingCommonPointResult.h>
#include <Applications/CAD/Intersection/Structures/NextCommonPointResult.h>

struct IntersectionResult
{
	IntersectionResult() : m_IntersectionFound(false) {}

	bool m_IntersectionFound;

	StartingCommonPointResult m_StartingPoint;

	std::vector<NextCommonPointResult> m_ForwardPoints;
	std::vector<NextCommonPointResult> m_BackwardsPoints;

	std::optional<LoopResult> m_LoopDetected = std::nullopt;
};
