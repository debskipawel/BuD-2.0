#include "RoughSphericalPathGenerator.h"

#include <Applications/CAD/Intersection/MultiIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/ResultConverter/IntersectionResultConverter.h>

#include <Applications/CAD/Path/Generator/CrossSection/CrossSection.h>

constexpr auto ROUGH_SPHERICAL_TOOL_RADIUS = 0.8f;

RoughSphericalPathGenerator::RoughSphericalPathGenerator(SceneCAD& scene, const std::vector<std::weak_ptr<SceneObjectCAD>>& surfaces)
	: AbstractPathGenerator(scene, surfaces, ROUGH_SPHERICAL_TOOL_RADIUS)
{
}

auto RoughSphericalPathGenerator::GeneratePaths(const MaterialBlockDetails& materialBlockDetails) -> MillingToolPath
{
	auto path = std::vector<dxm::Vector3>{ dxm::Vector3(0.0f, materialBlockDetails.m_Size.y + 2.0f, 0.0f)};

	auto eps = 0.1f;
	
	auto R = ROUGH_SPHERICAL_TOOL_RADIUS;
	auto T = 0.25f * R;
	auto D = 2.0f * R * sinf(acosf(1.0f - T / R));
	auto H = 3.0f * R;
	
	auto minX = materialBlockDetails.m_Position.x - (0.5f * materialBlockDetails.m_Size.x + R + eps);
	auto maxX = materialBlockDetails.m_Position.x + (0.5f * materialBlockDetails.m_Size.x + R + eps);

	auto minZ = materialBlockDetails.m_Position.z - (0.5f * materialBlockDetails.m_Size.z + R);
	auto maxZ = materialBlockDetails.m_Position.z + (0.5f * materialBlockDetails.m_Size.z + R);

	path.push_back(dxm::Vector3(minX, materialBlockDetails.m_Size.y + 2.0f, minZ));

	auto moveVectorX = dxm::Vector3(1.0f, 0.0f, 0.0f);
	auto moveVectorZ = dxm::Vector3(0.0f, 0.0f, 1.0f);

	auto moveForwardX = true;
	auto moveForwardZ = true;

	auto startY = materialBlockDetails.m_Size.y;
	auto endY = materialBlockDetails.m_StandHeight;

	auto yPassesCount = static_cast<int>(ceilf(fabsf(endY - startY) / H));
	auto zPassesCount = static_cast<int>(ceilf(fabsf(maxZ - minZ) / D));

	D = fabsf(maxZ - minZ) / zPassesCount;
	H = fabsf(endY - startY) / yPassesCount;
	T = R * (1.0f - cosf(asinf(D / R)));

	for (auto yPass = 0; yPass < yPassesCount; yPass++)
	{
		auto y = startY - (yPass + 1) * H;

		auto startZ = moveForwardZ ? minZ : maxZ;
		auto endZ = moveForwardZ ? maxZ : minZ;

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

			auto passResult = GenerateCrossSection(materialBlockDetails, startPosition, endPosition, horizontalPlane);

			std::copy(passResult.begin(), passResult.end(), std::back_inserter(path));

			moveForwardX = !moveForwardX;
		}

		m_SceneCAD.DeleteObject(*horizontalPlane.lock());

		moveForwardZ = !moveForwardZ;
	}

	return MillingToolPath(m_OffsetValue, path);
}

auto RoughSphericalPathGenerator::CreateHorizontalPlane(const MaterialBlockDetails& materialBlockDetails, float height) -> std::weak_ptr<SceneObjectCAD>
{
	auto& size = materialBlockDetails.m_Size;
	auto point = materialBlockDetails.m_Position + 0.5f * dxm::Vector3(-size.x, 0.0f, size.z) + height * dxm::Vector3::UnitY;
	
	return m_SceneCAD.CreateFinitePlane(point, dxm::Vector3::UnitX, -dxm::Vector3::UnitZ, materialBlockDetails.m_Size.x, materialBlockDetails.m_Size.z);
}

auto RoughSphericalPathGenerator::GenerateCrossSection(const MaterialBlockDetails& materialBlockDetails, const dxm::Vector3& startPosition, const dxm::Vector3& endPosition, std::weak_ptr<SceneObjectCAD> horizontalPlane) -> std::vector<dxm::Vector3>
{
	auto result = std::vector<dxm::Vector3>();

	auto dU = endPosition - startPosition;
	auto dV = dxm::Vector3::UnitY;

	auto widthU = dU.Length();
	auto widthV = materialBlockDetails.m_Size.y;

	dU.Normalize();
	dV.Normalize();

	auto verticalPlane = m_SceneCAD.CreateFinitePlane(dxm::Vector3(startPosition.x, 0.0f, startPosition.z), dU, dV, widthU, widthV);

	auto offsetSurfacesCopy = m_OffsetSurfaces;
	offsetSurfacesCopy.emplace_back(m_SceneCAD.CreateOffsetSurface(horizontalPlane, ROUGH_SPHERICAL_TOOL_RADIUS));

	auto crossSection = CrossSection(verticalPlane, offsetSurfacesCopy);
	auto polygon = crossSection.UpperBound();

	for (auto& polygonPoint : polygon)
	{
		auto point = m_Sampler->GetPoint(verticalPlane, polygonPoint.x, polygonPoint.y);

		point.y = max(point.y - ROUGH_SPHERICAL_TOOL_RADIUS, startPosition.y);

		result.push_back(point);
	}

	m_SceneCAD.DeleteObject(*verticalPlane.lock());

	return result;
}
