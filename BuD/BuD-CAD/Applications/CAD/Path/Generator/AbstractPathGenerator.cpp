#include "AbstractPathGenerator.h"

#include <Applications/CAD/Objects/PointBased/Surfaces/OffsetSurface.h>
#include <Applications/CAD/Intersection/Sampler/VisitorSampler.h>

AbstractPathGenerator::AbstractPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces, float millingToolRadius)
	: m_SceneCAD(scene), m_OffsetValue(millingToolRadius), m_Sampler(std::make_unique<VisitorSampler>())
{
	m_OffsetSurfaces.resize(surfaces.size());

	std::transform(surfaces.begin(), surfaces.end(), m_OffsetSurfaces.begin(),
		[&scene, millingToolRadius](std::weak_ptr<SceneObjectCAD> surface)
		{
			return std::make_shared<OffsetSurface>(scene.m_Scene, surface, millingToolRadius);
		}
	);
}
