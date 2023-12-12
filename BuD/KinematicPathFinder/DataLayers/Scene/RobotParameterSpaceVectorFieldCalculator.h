#pragma once

#include <DataLayers/Scene/ObstacleCollection.h>
#include <DataLayers/Scene/RobotParameterVectorField.h>

class RobotParameterSpaceVectorFieldCalculator
{
public:
	RobotParameterSpaceVectorFieldCalculator(ObstacleCollection& obstacleCollection);

	virtual auto CalculateVectorField(BuD::EditableTexture& parameterSpaceMap, const RobotArmConfiguration& targetConfiguration) -> RobotParameterVectorField;

protected:
	virtual auto WrapPixelParameters(std::pair<int, int> pixel, int width, int height) -> std::pair<int, int>;

	virtual auto GetNeighboursFloodFill4(std::pair<int, int> pixel, int width, int height) -> std::array<std::pair<int, int>, 4>;
	virtual auto GetNeighboursFloodFill8(std::pair<int, int> pixel, int width, int height) -> std::array<std::pair<int, int>, 8>;

	virtual auto GetCollidingObject(const RobotArmConfiguration& configuration) -> std::shared_ptr<Obstacle>;

	ObstacleCollection& m_ObstacleCollection;
};
