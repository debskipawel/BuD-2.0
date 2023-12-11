#include "DetailSphericalPathGenerator.h"

constexpr auto DETAIL_SPHERICAL_TOOL_RADIUS = 0.2f;

DetailSphericalPathGenerator::DetailSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces)
	: AbstractPathGenerator(scene, surfaces, DETAIL_SPHERICAL_TOOL_RADIUS)
{
}

auto DetailSphericalPathGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> MillingToolPath
{
	return MillingToolPath(m_OffsetValue, {});
}
