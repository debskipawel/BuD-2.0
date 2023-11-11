#pragma once

#include <BuD.h>

struct MaterialBlockParameters
{
public:
	MaterialBlockParameters(const dxm::Vector3& size, const dxm::Vector3& position, uint32_t resolutionWidth, uint32_t resolutionHeight);

	virtual bool operator==(const MaterialBlockParameters& other);
	virtual bool operator!=(const MaterialBlockParameters& other);

	/// <summary>
	/// Material block size in 3 dimentions in centimeters
	/// </summary>
	dxm::Vector3 m_Size;
	dxm::Vector3 m_Position;

	float m_MaxImmersion;

	uint32_t m_ResolutionWidth;
	uint32_t m_ResolutionHeight;

	static MaterialBlockParameters DEFAULT_PARAMETERS;
};
