#pragma once

#include <Applications/CAD/Path/Optimizer/AbstractPathOptimizer.h>

class ColinearPathOptimizer : public AbstractPathOptimizer
{
public:
	ColinearPathOptimizer(float lengthAccuracy = 0.003f, float angleAccuracy = 2.0f);

	virtual auto Optimize(const MillingToolPath& toolPath) const -> MillingToolPath override;

protected:
	float m_AngleAccuracy, m_LengthAccuracy;
};
