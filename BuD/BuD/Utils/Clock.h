#pragma once

#include <chrono>

namespace BuD
{
	class Clock
	{
		using time = std::chrono::high_resolution_clock;

	public:
		Clock() = delete;

		static void Init()
		{
			if (!s_Initialized)
			{
				s_Initialized = true;
				s_StartTimePoint = time::now();
			}
		}

		static float Now()
		{
			if (!s_Initialized)
			{
				return 0.0f;
			}

			auto now = time::now();

			return std::chrono::duration<float>(now - s_StartTimePoint).count();
		}

	protected:
		inline static bool s_Initialized = false;
		inline static std::chrono::steady_clock::time_point s_StartTimePoint;
	};
}
