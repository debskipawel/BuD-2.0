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
	auto borderPath = GeneratePathForModelBorder(materialBlockDetails);

	auto result = std::vector<dxm::Vector3>();

	std::copy(levelPath.begin(), levelPath.end(), std::back_inserter(result));
	std::copy(borderPath.begin(), borderPath.end(), std::back_inserter(result));

	return MillingToolPath(m_OffsetValue, result);
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
	auto xLimitDown = xEndDown;

	auto xStartUp = P.x - materialBlockDetails.m_Size.x - (STAND_FLAT_TOOL_RADIUS + 0.1f);
	auto xEndUp = STAND_FLAT_TOOL_RADIUS - 0.1f;
	auto xLimitUp = xEndUp;

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

		auto v = lowerBound.GetV(u);

		vStart = min(vStart, v);
		vEnd = min(vEnd, v);

		xStart = P.x + vStart * axisV.x;
		xEnd = P.x + vEnd * axisV.x;

		result.emplace_back(xStart, y, z);
		result.emplace_back(xEnd, y, z);

		if (!moveForwardX)
		{
			auto zNext = zStart - (zPass + 1) * dz;
			auto uNext = (P.z - zNext) / materialBlockDetails.m_Size.z;

			auto border = lowerBound.GetPath(u, uNext);

			for (auto& [uB, vB] : border)
			{
				auto p = dxm::Vector3(
					P.x + vB * axisV.x,
					y,
					P.z + uB * axisU.z
				);

				p.x = max(p.x, xLimitDown);

				result.push_back(p);
			}
		}

		moveForwardX = !moveForwardX;
	}

	moveForwardZ = !moveForwardZ;

	for (size_t zPass = 0; zPass < zPassesCount; zPass++)
	{
		auto z = zEnd + zPass * dz;

		auto xStart = moveForwardX ? xStartUp : xEndUp;
		auto xEnd = moveForwardX ? xEndUp : xStartUp;

		auto y = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

		auto u = (P.z - z) / materialBlockDetails.m_Size.z;
		auto vStart = (P.x - xStart) / materialBlockDetails.m_Size.x;
		auto vEnd = (P.x - xEnd) / materialBlockDetails.m_Size.x;

		auto v = upperBound.GetV(u);

		vStart = max(vStart, v);
		vEnd = max(vEnd, v);

		xStart = P.x + vStart * axisV.x;
		xEnd = P.x + vEnd * axisV.x;

		result.emplace_back(xStart, y, z);
		result.emplace_back(xEnd, y, z);

		if (moveForwardX)
		{
			auto zNext = zEnd + (zPass + 1) * dz;
			auto uNext = (P.z - zNext) / materialBlockDetails.m_Size.z;

			auto border = upperBound.GetPath(uNext, u);

			for (auto it = border.rbegin(); it != border.rend(); it++)
			{
				auto& [uB, vB] = *it;

				auto p = dxm::Vector3(
					P.x + vB * axisV.x,
					y,
					P.z + uB * axisU.z
				);

				p.x = min(p.x, xLimitUp);

				result.push_back(p);
			}
		}

		moveForwardX = !moveForwardX;
	}

	result.emplace_back(xEndUp, materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 1.0f, zStart);
	result.emplace_back(materialBlockDetails.m_Position.x, materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 1.0f, materialBlockDetails.m_Position.z);

	m_SceneCAD.DeleteObject(*horizontalPlane.lock());

	return result;
}

auto StandFlatToolPathGenerator::GeneratePathForModelBorder(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto result = std::vector<dxm::Vector3>();

	auto safeHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 1.0f;

	auto toolPosition = dxm::Vector3(
		materialBlockDetails.m_Position.x,
		safeHeight,
		materialBlockDetails.m_Position.z - 0.5f * materialBlockDetails.m_Size.z - STAND_FLAT_TOOL_RADIUS - 0.1f
	);

	result.emplace_back(toolPosition.x, toolPosition.y, toolPosition.z);
	result.emplace_back(toolPosition.x, materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight, toolPosition.z);

	auto planeAnchorPoint = dxm::Vector3(
		materialBlockDetails.m_Position.x - 0.5f * materialBlockDetails.m_Size.x,
		materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight,
		materialBlockDetails.m_Position.z - 0.5f * materialBlockDetails.m_Size.z
	);

	auto axisU = materialBlockDetails.m_Size.z * dxm::Vector3::UnitZ;
	auto axisV = materialBlockDetails.m_Size.x * dxm::Vector3::UnitX;

	auto horizontalPlane = m_SceneCAD.CreateFinitePlane(planeAnchorPoint, axisU, axisV);

	auto crossSection = CrossSection(horizontalPlane, m_OffsetSurfaces);

	auto lowerBound = crossSection.LowerBound();
	auto upperBound = crossSection.UpperBound();

	const auto& lowerBoundPoints = lowerBound.Points();
	const auto& upperBoundPoints = upperBound.Points();

	for (auto it = lowerBoundPoints.begin(); it != lowerBoundPoints.end(); it++)
	{
		auto& [u, v] = *it;

		toolPosition = planeAnchorPoint + u * axisU + v * axisV;

		result.push_back(toolPosition);
	}

	for (auto it = upperBoundPoints.rbegin(); it != upperBoundPoints.rend(); it++)
	{
		auto& [u, v] = *it;

		toolPosition = planeAnchorPoint + u * axisU + v * axisV;

		result.push_back(toolPosition);
	}

	result.emplace_back(toolPosition.x, safeHeight, toolPosition.z);
	result.emplace_back(materialBlockDetails.m_Position.x, safeHeight, materialBlockDetails.m_Position.z);

	m_SceneCAD.DeleteObject(*horizontalPlane.lock());

	return result;
}
