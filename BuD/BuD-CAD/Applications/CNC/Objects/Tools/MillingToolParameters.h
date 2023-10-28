#pragma once

struct MillingToolParameters
{
public:
	MillingToolParameters(float radius, float height);

	virtual bool operator==(const MillingToolParameters& other);
	virtual bool operator!=(const MillingToolParameters& other);

	/// <summary>
	/// Radius in centimeters
	/// </summary>
	float m_Radius;

	/// <summary>
	/// Milling height in centimeters
	/// </summary>
	float m_Height;
};
