#pragma once

#include <Scene/RobotArmConfiguration.h>

struct VectorFieldPoint
{
	size_t m_Distance;
	std::pair<int, int> m_Direction;
};

class RobotParameterSpaceVectorFieldCalculator
{
public:
	virtual auto CalculateVectorField(BuD::EditableTexture& parameterSpaceMap, std::pair<int, int> targetConfiguration) -> std::map<std::pair<int, int>, VectorFieldPoint>;
};
