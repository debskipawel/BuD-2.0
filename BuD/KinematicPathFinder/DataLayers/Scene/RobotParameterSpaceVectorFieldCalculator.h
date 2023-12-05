#pragma once

#include <DataLayers/Scene/RobotParameterVectorField.h>

class RobotParameterSpaceVectorFieldCalculator
{
public:
	virtual auto CalculateVectorField(BuD::EditableTexture& parameterSpaceMap, std::pair<int, int> targetConfiguration) -> RobotParameterVectorField;
};
