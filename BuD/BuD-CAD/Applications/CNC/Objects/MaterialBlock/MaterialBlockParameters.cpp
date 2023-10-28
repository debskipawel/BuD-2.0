#include "MaterialBlockParameters.h"

MaterialBlockParameters::MaterialBlockParameters(const dxm::Vector3& size)
	: m_Size(size)
{
}

bool MaterialBlockParameters::operator==(const MaterialBlockParameters& other)
{
	return m_Size == other.m_Size;
}

bool MaterialBlockParameters::operator!=(const MaterialBlockParameters& other)
{
	return !(*this == other);
}

MaterialBlockParameters MaterialBlockParameters::DEFAULT_PARAMETERS = MaterialBlockParameters({ 15.0f, 5.0f, 15.0f });
