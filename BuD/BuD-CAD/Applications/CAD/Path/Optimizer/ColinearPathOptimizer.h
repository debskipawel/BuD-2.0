#pragma once

#include <Applications/CAD/Path/Optimizer/AbstractPathOptimizer.h>

class ColinearPathOptimizer : public AbstractPathOptimizer
{
public:
	ColinearPathOptimizer(float accuracy = 1e-3f);

	virtual auto Optimize(const MillingToolPath& toolPath) const -> MillingToolPath override;

protected:
	float m_Accuracy;
};
