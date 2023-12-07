#include "DetailSphericalPathGenerator.h"

constexpr auto DETAIL_SPHERICAL_TOOL_RADIUS = 2.0f;

DetailSphericalPathGenerator::DetailSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces)
	: AbstractPathGenerator(scene, surfaces, DETAIL_SPHERICAL_TOOL_RADIUS)
{
}

auto DetailSphericalPathGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) const -> MillingToolPath
{
	return MillingToolPath(m_OffsetValue, {});
}
