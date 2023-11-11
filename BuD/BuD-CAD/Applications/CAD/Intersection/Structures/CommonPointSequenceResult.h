#pragma once

#include <optional>

#include <Applications/CAD/Intersection/Structures/LoopResult.h>
#include <Applications/CAD/Intersection/Structures/NextCommonPointResult.h>

struct CommonPointSequenceResult
{
	std::vector<NextCommonPointResult> m_Points;

	std::optional<LoopResult> m_LoopDetected = std::nullopt;
};
