#pragma once

#include <optional>

#include <Intersection/Structures/LoopResult.h>
#include <Intersection/Structures/NextCommonPointResult.h>

struct CommonPointSequenceResult
{
	std::vector<NextCommonPointResult> m_Points;

	std::optional<LoopResult> m_LoopDetected = std::nullopt;
};
