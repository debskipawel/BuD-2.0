#pragma once

#include <Applications/CAD/Path/Generator/AbstractPathGenerator.h>

class RoughSphericalPathGenerator : public AbstractPathGenerator
{
public:
	RoughSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces);

	virtual auto GeneratePaths(const MaterialBlockDetails& materialBlockDetails) const -> MillingToolPath override;
};
