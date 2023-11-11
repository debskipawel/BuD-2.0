#pragma once

#include <string>

struct FilterInfo
{
	FilterInfo()
		: m_TagFilter(std::string()), m_SkipPoints(false)
	{}

	std::string m_TagFilter;
	bool m_SkipPoints;

	bool operator==(const FilterInfo& other);
	bool operator!=(const FilterInfo& other);

	static FilterInfo DEFAULT_FILTER;
};
