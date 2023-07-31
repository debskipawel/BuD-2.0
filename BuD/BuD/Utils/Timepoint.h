#pragma once

#include <string>

namespace BuD
{
	class Timepoint
	{
	public:
		Timepoint() : Timepoint(0.0f) {}
		Timepoint(float time) : m_Time(time) {}
		Timepoint(const Timepoint& other) = default;
		Timepoint& operator=(const Timepoint& other) = default;
		Timepoint& operator=(Timepoint& other) = default;

		std::wstring Format() const;

		Timepoint operator+(const Timepoint& other) const;
		Timepoint operator-(const Timepoint& other) const;
		operator float();

	private:
		float m_Time;
	};
}
