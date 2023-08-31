#pragma once

#include <Raycasting/Ray.h>

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

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:
	Ray m_Ray;
	IntersectionResults m_LatestResults = {};
};
