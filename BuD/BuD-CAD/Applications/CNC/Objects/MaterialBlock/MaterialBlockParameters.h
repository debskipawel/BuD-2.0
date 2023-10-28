#pragma once

#include <BuD.h>

struct MaterialBlockParameters
{
public:
	MaterialBlockParameters(const dxm::Vector3& size);

	virtual bool operator==(const MaterialBlockParameters& other);
	virtual bool operator!=(const MaterialBlockParameters& other);

	/// <summary>
	/// Material block size in 3 dimentions in centimeters
	/// </summary>
	dxm::Vector3 m_Size;

	static MaterialBlockParameters DEFAULT_PARAMETERS;
};
