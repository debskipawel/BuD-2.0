#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

class BoundingPolygon
{
public:
	BoundingPolygon(const std::vector<dxm::Vector2>& polygonPoints);

	virtual auto GetV(float u) const -> float;
	virtual auto GetPath(float u1, float u2) -> std::vector<dxm::Vector2>;

	virtual auto Points() const -> const std::vector<dxm::Vector2>&;

protected:
	std::vector<dxm::Vector2> m_PolygonPoints;
};
