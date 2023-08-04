#pragma once

#include <Scene/SceneObjectCAD.h>

#include <Objects/Torus.h>
#include <Objects/Cube.h>

class AbstractVisitor
{
public:
	virtual void Visit(SceneObjectCAD& object);

	virtual void Visit(Torus& torus) = 0;
	virtual void Visit(Cube& cube) = 0;
};
