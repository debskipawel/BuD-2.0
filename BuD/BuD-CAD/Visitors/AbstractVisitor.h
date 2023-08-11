#pragma once

#include <Scene/SceneObjectCAD.h>

#include <Objects/CAD/Point.h>
#include <Objects/CAD/Torus.h>

class AbstractVisitor
{
public:
	virtual void Visit(std::weak_ptr<SceneObjectCAD> object);

	virtual void Visit(Point& point) {}
	virtual void Visit(Torus& torus) {}

protected:
	std::weak_ptr<SceneObjectCAD> m_Caller;
};
