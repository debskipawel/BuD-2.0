#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>
#include <Applications/CAD/Intersection/Sampler/ISampler.h>

#include <Applications/CAD/Path/MillingToolPath.h>
#include <Applications/CAD/Path/MaterialBlockDetails.h>

class AbstractPathGenerator
{
public:
	AbstractPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces, float millingToolRadius);

	virtual auto GeneratePaths(const MaterialBlockDetails& materialBlockDetails) const -> MillingToolPath = 0;

protected:
	float m_OffsetValue;

	std::vector<std::shared_ptr<SceneObjectCAD>> m_OffsetSurfaces;
	std::unique_ptr<ISampler> m_Sampler;
};
