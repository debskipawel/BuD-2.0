#include "bud_pch.h"
#include "Timepoint.h"

#include <stdio.h>
#include <wchar.h>

namespace BuD
{
	std::wstring Timepoint::Format() const
	{
		auto seconds = static_cast<int>(m_Time);
		auto minutes = seconds / 60;
		auto hours = minutes / 60;
		auto millisecs = static_cast<int>((m_Time - seconds) * 1000); 

		wchar_t buffer[15] = {};
		swprintf_s(buffer, L"%02d:%02d:%02d:%03d\0", hours, minutes, seconds, millisecs);
		
		return std::wstring(buffer);
	}
	
	Timepoint Timepoint::operator+(const Timepoint& other) const
	{
		return Timepoint(m_Time + other.m_Time);
	}
	
	Timepoint Timepoint::operator-(const Timepoint& other) const
	{
		return Timepoint(m_Time - other.m_Time);
	}
	
	Timepoint::operator float()
	{
		return m_Time;
	}
}
