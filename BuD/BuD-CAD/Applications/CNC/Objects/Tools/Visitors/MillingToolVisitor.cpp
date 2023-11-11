#include "MillingToolVisitor.h"

void MillingToolVisitor::Visit(MillingTool& millingTool)
{
	millingTool.Accept(*this);
}
