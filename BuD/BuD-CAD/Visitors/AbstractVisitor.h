#pragma once

#include <Scene/SceneObjectCAD.h>

#include <Objects/CAD/Point.h>
#include <Objects/CAD/Torus.h>
#include <Objects/CAD/PointBased/Curve/BezierCurveC0.h>

class AbstractVisitor
{
public:
	virtual ~AbstractVisitor() = default;

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object);

	virtual void Visit(Point& point) {}
	virtual void Visit(Torus& torus) {}

	virtual void Visit(BezierCurveC0& curve) {}

protected:
	std::weak_ptr<SceneObjectCAD> m_Caller;
};
