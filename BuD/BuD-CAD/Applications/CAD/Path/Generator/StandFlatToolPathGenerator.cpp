#include "StandFlatToolPathGenerator.h"

constexpr auto STAND_FLAT_TOOL_RADIUS = 5.0f;

StandFlatToolPathGenerator::StandFlatToolPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces)
	: AbstractPathGenerator(scene, surfaces, STAND_FLAT_TOOL_RADIUS)
{
}

auto StandFlatToolPathGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) const -> MillingToolPath
{
	return MillingToolPath(m_OffsetValue, {});
}
