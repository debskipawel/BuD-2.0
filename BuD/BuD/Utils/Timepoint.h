#pragma once

#include <string>

namespace BuD
{
	class Timepoint
	{
	public:
		Timepoint(float time) : m_Time(time) {}
		Timepoint(const Timepoint& other) = default;
		Timepoint& operator=(const Timepoint& other) = default;
		Timepoint& operator=(Timepoint& other) = default;

		std::wstring Format();

		Timepoint operator+(const Timepoint& other);
		Timepoint operator-(const Timepoint& other);
		operator float();

	private:
		float m_Time;
	};
}
