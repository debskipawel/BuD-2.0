#pragma once

#include <Applications/CAD/MillingToolGenerator/MillingToolPath.h>

class AbstractPathOptimizer
{
public:
	virtual auto Optimize(const MillingToolPath& toolPath) const -> MillingToolPath = 0;
};
