#pragma once

#include <Scene/SceneObjectCAD.h>

#include <Objects/CAD/Point.h>
#include <Objects/CAD/Torus.h>
#include <Objects/Additional/Cube.h>

class AbstractVisitor
{
public:
	virtual void Visit(SceneObjectCAD& object);

	virtual void Visit(Point& point) {}
	virtual void Visit(Torus& torus) {}
	virtual void Visit(Cube& cube) {}
};
