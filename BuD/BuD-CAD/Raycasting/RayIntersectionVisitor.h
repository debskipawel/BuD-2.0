#pragma once

#include "Ray.h"

#include <Visitors/AbstractVisitor.h>

struct IntersectionResults
{
	bool m_Hit = false;
	float m_Distance = 0.0f;
};

class RayIntersectionVisitor : public AbstractVisitor
{
public:
	RayIntersectionVisitor(const Ray& ray);

	inline IntersectionResults GetLatestResults() { return m_LatestResults; }

	virtual void Visit(SceneObjectCAD& object) override;

	virtual void Visit(Point& point) override;
	virtual void Visit(Torus& torus) override;
	virtual void Visit(Cube& cube) override;

protected:
	Ray m_Ray;
	IntersectionResults m_LatestResults = {};
};
