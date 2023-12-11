#pragma once

#include <Scene/RobotArmConfiguration.h>

struct VectorFieldPoint
{
	size_t m_Distance;
	std::pair<int, int> m_Direction;
};

class RobotParameterVectorField
{
public:
	RobotParameterVectorField(size_t width);

	virtual auto Contains(std::pair<int, int> point) const noexcept -> bool;
	virtual auto Get(std::pair<int, int> point) -> VectorFieldPoint&;

	virtual auto Insert(std::pair<int, int> point, const VectorFieldPoint& vectorFieldPoint) -> void;

	virtual auto ForEach(std::function<void(std::pair<int, int>, VectorFieldPoint)> handler) const -> void;

	virtual auto operator[](std::pair<int, int> point) -> VectorFieldPoint&;

protected:
	std::function<int(std::pair<int, int>)> m_HashFunction;
	std::function<std::pair<int, int>(int)> m_ReverseHashFunction;

	std::unordered_map<int, VectorFieldPoint> m_ParameterSpacePathMap;
};
