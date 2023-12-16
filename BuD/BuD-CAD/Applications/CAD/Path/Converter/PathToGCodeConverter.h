#pragma once

#include <Applications/CAD/Path/MillingToolPath.h>

#include <GCodeProgram.h>

class PathToGCodeConverter
{
public:
	virtual auto Convert(const MillingToolPath& millingToolPath) -> GCP::GCodeProgram;
};
