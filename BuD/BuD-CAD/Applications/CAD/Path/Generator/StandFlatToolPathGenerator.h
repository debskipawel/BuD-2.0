#pragma once

#include <Applications/CAD/Path/Generator/AbstractPathGenerator.h>

class StandFlatToolPathGenerator : public AbstractPathGenerator
{
public:
	StandFlatToolPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces);

	virtual auto GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> MillingToolPath override;
};
