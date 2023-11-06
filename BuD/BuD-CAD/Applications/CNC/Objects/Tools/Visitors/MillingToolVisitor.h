#pragma once

#include <Applications/CNC/Objects/Tools/FlatMillingTool.h>
#include <Applications/CNC/Objects/Tools/SphericalMillingTool.h>

class MillingToolVisitor
{
public:
	virtual void Visit(MillingTool& millingTool);

	virtual void Visit(FlatMillingTool& millingTool) = 0;
	virtual void Visit(SphericalMillingTool& millingTool) = 0;
};
