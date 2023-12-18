#include "StandFlatToolPathGenerator.h"

#include <Applications/CAD/Path/Generator/CrossSection/CrossSection.h>

constexpr auto STAND_FLAT_TOOL_RADIUS = 0.5f;

StandFlatToolPathGenerator::StandFlatToolPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces)
	: AbstractPathGenerator(scene, surfaces, STAND_FLAT_TOOL_RADIUS)
{
}

auto StandFlatToolPathGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> MillingToolPath
{
	auto levelPath = GeneratePathForBlockLeveling(materialBlockDetails);

	return MillingToolPath(m_OffsetValue, levelPath);
}

auto StandFlatToolPathGenerator::GeneratePathForBlockLeveling(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto result = std::vector<dxm::Vector3>();

	result.emplace_back(materialBlockDetails.m_Position.x, materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 1.0f, materialBlockDetails.m_Position.z);

	auto P = materialBlockDetails.m_Position + dxm::Vector3(0.5f * materialBlockDetails.m_Size.x, materialBlockDetails.m_StandHeight, 0.5f * materialBlockDetails.m_Size.z);

	auto axisU = -materialBlockDetails.m_Size.z * dxm::Vector3::UnitZ;
	auto axisV = -materialBlockDetails.m_Size.x * dxm::Vector3::UnitX;

	auto horizontalPlane = m_SceneCAD.CreateFinitePlane(P, axisU, axisV);

	auto crossSection = CrossSection(horizontalPlane, m_OffsetSurfaces);

	auto lowerBound = crossSection.LowerBound();
	auto upperBound = crossSection.UpperBound();

	auto dz = 1.9f * STAND_FLAT_TOOL_RADIUS;

	auto zStart = P.z + STAND_FLAT_TOOL_RADIUS + 0.1f;
	auto zEnd = P.z - materialBlockDetails.m_Size.z - (STAND_FLAT_TOOL_RADIUS + 0.1f);

	auto zPassesCount = static_cast<int>(ceilf(fabsf(zEnd - zStart) / dz));
	dz = fabsf(zEnd - zStart) / zPassesCount;

	auto xStartDown = P.x + (STAND_FLAT_TOOL_RADIUS + 0.1f);
	auto xEndDown = 0.1f - STAND_FLAT_TOOL_RADIUS;

	auto xStartUp = P.x - materialBlockDetails.m_Size.x - (STAND_FLAT_TOOL_RADIUS + 0.1f);
	auto xEndUp = STAND_FLAT_TOOL_RADIUS - 0.1f;

	auto moveForwardX = false;
	auto moveForwardZ = false;

	result.emplace_back(xStartDown, materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 1.0f, zStart);

	for (size_t zPass = 0; zPass < zPassesCount; zPass++)
	{
		auto z = zStart - zPass * dz;

		auto xStart = moveForwardX ? xEndDown : xStartDown;
		auto xEnd = moveForwardX ? xStartDown : xEndDown;

		auto y = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

		auto u = (P.z - z) / materialBlockDetails.m_Size.z;
		auto vStart = (P.x - xStart) / materialBlockDetails.m_Size.x;
		auto vEnd = (P.x - xEnd) / materialBlockDetails.m_Size.x;

		result.emplace_back(xStart, y, z);
		result.emplace_back(xEnd, y, z);

		moveForwardX = !moveForwardX;
	}

	moveForwardZ = !moveForwardZ;

	for (size_t zPass = 0; zPass < zPassesCount; zPass++)
	{
		auto z = zEnd + zPass * dz;

		auto xStart = moveForwardX ? xStartUp : xEndUp;
		auto xEnd = moveForwardX ? xEndUp : xStartUp;

		auto y = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

		result.emplace_back(xStart, y, z);
		result.emplace_back(xEnd, y, z);

		moveForwardX = !moveForwardX;
	}

	result.emplace_back(xEndUp, materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 1.0f, zStart);
	result.emplace_back(materialBlockDetails.m_Position.x, materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 1.0f, materialBlockDetails.m_Position.z);

	return result;
}

auto StandFlatToolPathGenerator::GeneratePathForModelBorder(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto result = std::vector<dxm::Vector3>();

	return result;
}
