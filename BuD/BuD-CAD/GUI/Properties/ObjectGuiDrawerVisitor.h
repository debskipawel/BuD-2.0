#pragma once

#include <Visitors/AbstractVisitor.h>

class ObjectGuiDrawerVisitor : public AbstractVisitor
{
public:
	virtual void Visit(Torus& torus) override;
	virtual void Visit(Cube& cube) override;

protected:
	virtual bool DrawGuiForTransform(SceneObjectCAD& object);
};
