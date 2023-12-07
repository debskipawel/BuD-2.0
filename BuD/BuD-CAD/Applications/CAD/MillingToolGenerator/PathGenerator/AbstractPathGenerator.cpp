#include "AbstractPathGenerator.h"

#include <Applications/CAD/Objects/PointBased/Surfaces/OffsetSurface.h>

AbstractPathGenerator::AbstractPathGenerator(BuD::Scene& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces, float millingToolRadius)
	: m_OffsetValue(millingToolRadius)
{
	m_OffsetSurfaces.resize(surfaces.size());

	std::transform(surfaces.begin(), surfaces.end(), m_OffsetSurfaces.begin(),
		[&scene, millingToolRadius](std::weak_ptr<SceneObjectCAD> surface)
		{
			return std::make_shared<OffsetSurface>(scene, surface, millingToolRadius);
		}
	);
}
