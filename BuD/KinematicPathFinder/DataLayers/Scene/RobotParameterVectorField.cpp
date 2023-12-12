#include "RobotParameterVectorField.h"

RobotParameterVectorField::RobotParameterVectorField(size_t width)
{
	m_HashFunction = [width](std::pair<int, int> key)
	{
		auto [x, y] = key;

		return y * width + x;
	};

	m_ReverseHashFunction = [width](int value)
	{
		auto y = value / width;
		auto x = value % width;

		return std::make_pair(x, y);
	};
}

auto RobotParameterVectorField::Contains(std::pair<int, int> point) const noexcept -> bool
{
	auto hash = m_HashFunction(point);

	return m_ParameterSpacePathMap.contains(hash);
}

auto RobotParameterVectorField::Get(std::pair<int, int> point) -> VectorFieldPoint&
{
	auto hash = m_HashFunction(point);

	return m_ParameterSpacePathMap.at(hash);
}

auto RobotParameterVectorField::Insert(std::pair<int, int> point, const VectorFieldPoint& vectorFieldPoint) -> void
{
	auto hash = m_HashFunction(point);

	m_ParameterSpacePathMap[hash] = vectorFieldPoint;
}

auto RobotParameterVectorField::ForEach(std::function<void(std::pair<int, int>, VectorFieldPoint)> handler) const -> void
{
	for (const auto& [hash, vectorFieldPoint] : m_ParameterSpacePathMap)
	{
		auto pair = m_ReverseHashFunction(hash);

		handler(pair, vectorFieldPoint);
	}
}

auto RobotParameterVectorField::operator[](std::pair<int, int> point) -> VectorFieldPoint&
{
	if (!Contains(point))
	{
		Insert(point, VectorFieldPoint());
	}

	return Get(point);
}
