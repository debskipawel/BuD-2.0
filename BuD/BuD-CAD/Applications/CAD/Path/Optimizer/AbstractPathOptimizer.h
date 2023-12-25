#pragma once

#include <Applications/CAD/Path/MillingToolPath.h>

class AbstractPathOptimizer
{
public:
	virtual auto Optimize(const MillingToolPath& toolPath) const -> MillingToolPath = 0;
};
