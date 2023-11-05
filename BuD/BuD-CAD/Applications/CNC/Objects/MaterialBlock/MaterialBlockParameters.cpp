#include "MaterialBlockParameters.h"

MaterialBlockParameters::MaterialBlockParameters(const dxm::Vector3& size, const dxm::Vector3& position, uint32_t resolutionWidth, uint32_t resolutionHeight)
	: m_Size(size), m_Position(position), m_ResolutionWidth(resolutionWidth), m_ResolutionHeight(resolutionHeight)
{
}

bool MaterialBlockParameters::operator==(const MaterialBlockParameters& other)
{
	return m_Size == other.m_Size && m_ResolutionWidth == other.m_ResolutionWidth && m_ResolutionHeight == other.m_ResolutionHeight;
}

bool MaterialBlockParameters::operator!=(const MaterialBlockParameters& other)
{
	return !(*this == other);
}

MaterialBlockParameters MaterialBlockParameters::DEFAULT_PARAMETERS = MaterialBlockParameters({ 15.0f, 5.0f, 15.0f }, dxm::Vector3::Zero, 1024U, 1024U);
