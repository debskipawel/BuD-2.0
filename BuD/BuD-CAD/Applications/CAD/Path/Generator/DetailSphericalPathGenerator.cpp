#include "DetailSphericalPathGenerator.h"

#include <Applications/CAD/Path/Generator/CrossSection/CrossSection.h>

constexpr auto DETAIL_SPHERICAL_TOOL_RADIUS = 0.4f;

DetailSphericalPathGenerator::DetailSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces)
	: AbstractPathGenerator(scene, surfaces, DETAIL_SPHERICAL_TOOL_RADIUS)
{
}

auto DetailSphericalPathGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> MillingToolPath
{
	auto safeHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 2.0f;

	auto path = std::vector<dxm::Vector3>{ dxm::Vector3(materialBlockDetails.m_Position.x, safeHeight, materialBlockDetails.m_Position.z) };

	auto generalDetailPath = GenerateGeneralPathsForDetailMilling(materialBlockDetails);

	std::copy(generalDetailPath.begin(), generalDetailPath.end(), std::back_inserter(path));
	
	path.emplace_back(materialBlockDetails.m_Position.x, safeHeight, materialBlockDetails.m_Position.z);

	return MillingToolPath(m_OffsetValue, path);
}

auto DetailSphericalPathGenerator::GenerateGeneralPathsForDetailMilling(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto safeHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 2.0f;

	auto path = std::vector<dxm::Vector3>();

	auto eps = 0.1f;

	auto R = DETAIL_SPHERICAL_TOOL_RADIUS;
	auto T = 0.05f * R;
	auto D = 2.0f * R * sinf(acosf(1.0f - T / R));

	auto minX = materialBlockDetails.m_Position.x - (0.5f * materialBlockDetails.m_Size.x + R + eps);
	auto maxX = materialBlockDetails.m_Position.x + (0.5f * materialBlockDetails.m_Size.x + R + eps);

	auto minZ = materialBlockDetails.m_Position.z - (0.5f * materialBlockDetails.m_Size.z + R);
	auto maxZ = materialBlockDetails.m_Position.z + (0.5f * materialBlockDetails.m_Size.z + R);

	auto toolPosition = dxm::Vector3(minX, safeHeight, minZ);

	path.push_back(toolPosition);

	auto planeAnchorPoint = dxm::Vector3(minX, materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight, minZ);
	
	auto horizontalPlaneAxisU = (maxZ - minZ) * dxm::Vector3::UnitZ;
	auto horizontalPlaneAxisV = (maxX - minX) * dxm::Vector3::UnitX;

	auto horizontalPlane = m_SceneCAD.CreateFinitePlane(planeAnchorPoint, dxm::Vector3::UnitZ, dxm::Vector3::UnitX, maxZ - minZ, maxX - minX);

	auto horizontalCrossSection = CrossSection(horizontalPlane, m_OffsetSurfaces);
	
	auto horizontalLowerBound = horizontalCrossSection.LowerBound();
	auto horizontalUpperBound = horizontalCrossSection.UpperBound();

	auto moveForwardX = true;

	auto zPassesCount = static_cast<int>(ceilf(fabsf(maxZ - minZ) / D));

	D = fabsf(maxZ - minZ) / zPassesCount;
	T = R * (1.0f - cosf(asinf(D / R)));

	auto y = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

	auto startZ = minZ;
	auto endZ = maxZ;

	auto currentMoveZ = dxm::Vector3(0.0f, 0.0f, endZ - startZ);
	currentMoveZ.Normalize();

	for (auto zPass = 0; zPass < zPassesCount; zPass++)
	{
		auto z = startZ + zPass * D * currentMoveZ.z;

		auto startX = moveForwardX ? minX : maxX;
		auto endX = moveForwardX ? maxX : minX;

		auto startPosition = dxm::Vector3(startX, y, z);
		auto endPosition = dxm::Vector3(endX, y, z);

		auto passResult = FindCrossSectionPath(materialBlockDetails, startPosition, endPosition);

		if (passResult.empty())
		{
			continue;
		}

		std::copy(passResult.begin(), passResult.end(), std::back_inserter(path));

		if (zPass + 1 < zPassesCount)
		{
			auto zNext = startZ + (zPass + 1) * D * currentMoveZ.z;

			auto uCurrent = (z - minZ) / (maxZ - minZ);
			auto uNext = (zNext - minZ) / (maxZ - minZ);

			auto& polygon = moveForwardX ? horizontalUpperBound : horizontalLowerBound;

			auto transitionPath = polygon.GetPath(uCurrent, uNext);

			for (auto& parameter : transitionPath)
			{
				auto point = m_Sampler->GetPoint(horizontalPlane, parameter.x, parameter.y);

				point.y = max(point.y - DETAIL_SPHERICAL_TOOL_RADIUS, y);

				path.push_back(point);
			}
		}

		toolPosition = path.back();

		moveForwardX = !moveForwardX;
	}

	path.emplace_back(toolPosition.x, safeHeight, toolPosition.z);

	return path;
}

auto DetailSphericalPathGenerator::GeneratePathsForIntersectionBorders(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto safeHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 2.0f;

	auto result = std::vector<dxm::Vector3>();

	return result;
}

auto DetailSphericalPathGenerator::FindCrossSectionPath(const MaterialBlockDetails& materialBlockDetails, const dxm::Vector3& startPosition, const dxm::Vector3& endPosition) -> std::vector<dxm::Vector3>
{
	auto result = std::vector<dxm::Vector3>();

	auto dU = endPosition - startPosition;
	auto dV = dxm::Vector3::UnitY;

	auto widthU = dU.Length();
	auto widthV = materialBlockDetails.m_Size.y;

	dU.Normalize();
	dV.Normalize();

	auto verticalPlane = m_SceneCAD.CreateFinitePlane(dxm::Vector3(startPosition.x, 0.0f, startPosition.z), dU, dV, widthU, widthV);

	auto crossSection = CrossSection(verticalPlane, m_OffsetSurfaces);
	auto polygon = crossSection.UpperBound();

	const auto& upperBoundPoints = polygon.Points();

	if (!upperBoundPoints.empty())
	{
		for (auto& polygonPoint : upperBoundPoints)
		{
			auto point = m_Sampler->GetPoint(verticalPlane, polygonPoint.x, polygonPoint.y);

			point.y = max(point.y - DETAIL_SPHERICAL_TOOL_RADIUS, startPosition.y);

			result.push_back(point);
		}
	}

	m_SceneCAD.DeleteObject(*verticalPlane.lock());

	return result;
}

auto DetailSphericalPathGenerator::CreateHorizontalStandPlane(const MaterialBlockDetails& materialBlockDetails) -> std::weak_ptr<SceneObjectCAD>
{
	auto eps = 0.1f;

	auto R = DETAIL_SPHERICAL_TOOL_RADIUS;

	auto minX = materialBlockDetails.m_Position.x - (0.5f * materialBlockDetails.m_Size.x + R + eps);
	auto maxX = materialBlockDetails.m_Position.x + (0.5f * materialBlockDetails.m_Size.x + R + eps);

	auto minZ = materialBlockDetails.m_Position.z - (0.5f * materialBlockDetails.m_Size.z + R);
	auto maxZ = materialBlockDetails.m_Position.z + (0.5f * materialBlockDetails.m_Size.z + R);

	auto y = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

	auto planeAnchorPoint = dxm::Vector3(minX, y, minZ);

	return m_SceneCAD.CreateFinitePlane(planeAnchorPoint, dxm::Vector3::UnitZ, dxm::Vector3::UnitX, maxZ - minZ, maxX - minX);
}
