#include "MillingToolParameters.h"

MillingToolParameters::MillingToolParameters(float radius, float height)
	: m_Radius(radius), m_Height(height)
{
}

bool MillingToolParameters::operator==(const MillingToolParameters& other)
{
	return m_Radius == other.m_Radius && m_Height == other.m_Height;
}

bool MillingToolParameters::operator!=(const MillingToolParameters& other)
{
	return !(*this == other);
}
