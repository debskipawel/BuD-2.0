#pragma once

#include <Applications/CAD/Path/Generator/CrossSection/Edge.h>

class ActiveEdgeTable
{
public:
	virtual auto Add(const Edge& edge) -> void;

	virtual auto CurrentUpperBound() -> std::optional<Edge>;
	virtual auto CurrentLowerBound() -> std::optional<Edge>;

	virtual auto GetFirstIntersecting(const Edge& edge, float lowerBound = 0.0f, float upperBound = 1.0f) -> std::pair<std::optional<Edge>, float>;

	virtual auto Update(float u) -> void;

protected:
	virtual auto Intersection(const Edge& e1, const Edge& e2) -> std::optional<float>;

	float m_U;

	std::vector<Edge> m_ActiveEdgeList;
};
