#include "RoughSphericalPathGenerator.h"

constexpr auto ROUGH_SPHERICAL_TOOL_RADIUS = 5.0f;

RoughSphericalPathGenerator::RoughSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces)
	: AbstractPathGenerator(scene, surfaces, ROUGH_SPHERICAL_TOOL_RADIUS)
{
}

auto RoughSphericalPathGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) const -> MillingToolPath
{
	return MillingToolPath(m_OffsetValue, {});
}
