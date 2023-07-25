#pragma once

#include <Utils/Timepoint.h>

namespace BuD
{
	struct FramePerformanceData
	{
		float m_FrameTime;
		float m_FrameRate;

		unsigned int m_DrawCalls;
		unsigned int m_InstancesDrawn;
	};
}
