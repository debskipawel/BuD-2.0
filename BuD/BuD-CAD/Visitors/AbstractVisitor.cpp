#include "AbstractVisitor.h"

void AbstractVisitor::Visit(SceneObjectCAD& object)
{
	object.Accept(*this);
}
