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

	auto eps = 0.1f;

	auto R = DETAIL_SPHERICAL_TOOL_RADIUS;
	auto T = 0.02f * R;
	auto D = 2.0f * R * sinf(acosf(1.0f - T / R));

	auto minX = materialBlockDetails.m_Position.x - (0.5f * materialBlockDetails.m_Size.x + R + eps);
	auto maxX = materialBlockDetails.m_Position.x + (0.5f * materialBlockDetails.m_Size.x + R + eps);

	auto minZ = materialBlockDetails.m_Position.z - (0.5f * materialBlockDetails.m_Size.z + R);
	auto maxZ = materialBlockDetails.m_Position.z + (0.5f * materialBlockDetails.m_Size.z + R);

	auto toolPosition = dxm::Vector3(minX, safeHeight, minZ);

	path.push_back(toolPosition);

	auto moveForwardX = true;

	auto zPassesCount = static_cast<int>(ceilf(fabsf(maxZ - minZ) / D));

	D = fabsf(maxZ - minZ) / zPassesCount;
	T = R * (1.0f - cosf(asinf(D / R)));

	auto y = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

	auto startZ = minZ;
	auto endZ = maxZ;

	auto currentMoveZ = dxm::Vector3(0.0f, 0.0f, endZ - startZ);
	currentMoveZ.Normalize();

	auto horizontalPlane = CreateHorizontalPlane(materialBlockDetails, y);

	for (auto zPass = 0; zPass < zPassesCount; zPass++)
	{
		auto z = startZ + zPass * D * currentMoveZ.z;

		auto startX = moveForwardX ? minX : maxX;
		auto endX = moveForwardX ? maxX : minX;

		auto startPosition = dxm::Vector3(startX, y, z);
		auto endPosition = dxm::Vector3(endX, y, z);

		toolPosition = endPosition;

		auto passResult = GenerateCrossSection(materialBlockDetails, startPosition, endPosition, horizontalPlane);

		std::copy(passResult.begin(), passResult.end(), std::back_inserter(path));

		moveForwardX = !moveForwardX;
	}

	m_SceneCAD.DeleteObject(*horizontalPlane.lock());

	path.emplace_back(toolPosition.x, safeHeight, toolPosition.z);
	path.emplace_back(materialBlockDetails.m_Position.x, safeHeight, materialBlockDetails.m_Position.z);

	return MillingToolPath(m_OffsetValue, path);
}

auto DetailSphericalPathGenerator::CreateHorizontalPlane(const MaterialBlockDetails& materialBlockDetails, float height) -> std::weak_ptr<SceneObjectCAD>
{
	auto& size = materialBlockDetails.m_Size;
	auto point = materialBlockDetails.m_Position + 0.5f * dxm::Vector3(-size.x, 0.0f, size.z) + height * dxm::Vector3::UnitY;

	return m_SceneCAD.CreateFinitePlane(point, dxm::Vector3::UnitX, -dxm::Vector3::UnitZ, materialBlockDetails.m_Size.x, materialBlockDetails.m_Size.z);
}

auto DetailSphericalPathGenerator::GenerateCrossSection(const MaterialBlockDetails& materialBlockDetails, const dxm::Vector3& startPosition, const dxm::Vector3& endPosition, std::weak_ptr<SceneObjectCAD> horizontalPlane) -> std::vector<dxm::Vector3>
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

	result.push_back(startPosition);

	for (auto& polygonPoint : polygon.Points())
	{
		auto point = m_Sampler->GetPoint(verticalPlane, polygonPoint.x, polygonPoint.y);

		point.y = max(point.y - DETAIL_SPHERICAL_TOOL_RADIUS, startPosition.y);

		result.push_back(point);
	}

	result.push_back(endPosition);

	m_SceneCAD.DeleteObject(*verticalPlane.lock());

	return result;
}
