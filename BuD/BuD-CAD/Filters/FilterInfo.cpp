#include "FilterInfo.h"

FilterInfo FilterInfo::DEFAULT_FILTER = {};

bool FilterInfo::operator==(const FilterInfo& other)
{
	return m_SkipPoints == other.m_SkipPoints && m_TagFilter == other.m_TagFilter;
}

bool FilterInfo::operator!=(const FilterInfo& other)
{
	return !(*this == other);
}
