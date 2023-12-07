#pragma once

#include <Applications/CAD/Scene/SceneObjectCAD.h>
#include <Applications/CAD/MillingToolGenerator/MillingToolPath.h>

class AbstractPathGenerator
{
public:
	AbstractPathGenerator(BuD::Scene& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces, float millingToolRadius);

	virtual auto GeneratePaths() -> MillingToolPath;

protected:
	float m_OffsetValue;

	std::vector<std::shared_ptr<SceneObjectCAD>> m_OffsetSurfaces;
};
