#pragma once

#include <Utils/Timepoint.h>

namespace BuD
{
	struct FramePerformanceData
	{
		Timepoint m_FrameTime;
		Timepoint m_FrameRate;

		unsigned int m_DrawCalls;
		unsigned int m_InstancesDrawn;
	};
}
