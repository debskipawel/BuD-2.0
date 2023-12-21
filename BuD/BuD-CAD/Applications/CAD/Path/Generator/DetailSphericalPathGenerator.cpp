#include "DetailSphericalPathGenerator.h"

#include <Applications/CAD/Path/Generator/CrossSection/CrossSection.h>

#include <Applications/CAD/Intersection/MultiIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/ResultConverter/IntersectionResultConverter.h>

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
	auto buttonBodyIntersectionPath = GeneratePathsForButtonBodyIntersection(materialBlockDetails);
	auto screwBodyIntersectionPath = GeneratePathsForScrewButtonIntersection(materialBlockDetails);

	std::copy(buttonBodyIntersectionPath.begin(), buttonBodyIntersectionPath.end(), std::back_inserter(path));
	std::copy(screwBodyIntersectionPath.begin(), screwBodyIntersectionPath.end(), std::back_inserter(path));
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
	auto T = 0.01f * R;
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

auto DetailSphericalPathGenerator::GeneratePathsForButtonBodyIntersection(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto result = std::vector<dxm::Vector3>();

	auto safeHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 2.0f;
	auto minHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

	auto startingParameters = std::vector<dxm::Vector2>{ { 0.25f, 0.25f }, { 0.75f, 0.25f }, { 0.75f, 0.25f }, { 0.75f, 0.75f } };

	auto surface1 = std::find_if(m_OffsetSurfaces.begin(), m_OffsetSurfaces.end(), 
		[](std::shared_ptr<SceneObjectCAD> object)
		{
			auto& tag = object->m_Tag;
			return tag.find("Button") != std::string::npos;
		}
	);

	auto surface2 = std::find_if(m_OffsetSurfaces.begin(), m_OffsetSurfaces.end(),
		[](std::shared_ptr<SceneObjectCAD> object)
		{
			auto& tag = object->m_Tag;
			return tag.find("Body") != std::string::npos;
		}
	);

	if (surface1 == m_OffsetSurfaces.end() || surface2 == m_OffsetSurfaces.end())
	{
		return result;
	}

	auto intersectionParameters = IntersectionAlgorithmParameters();
	intersectionParameters.m_UseCursorAsStartingPoint = false;
	intersectionParameters.m_PointDistance = 0.01f;

	auto intersectionAlgorithm = MultiIntersectionAlgorithm(intersectionParameters, *surface1, *surface2);

	auto intersection = intersectionAlgorithm.Find();

	auto converter = IntersectionResultConverter();

	auto [points1, points2] = converter.ToPointVectors(intersection);

	auto intersectionCurve = std::vector<dxm::Vector3>();

	std::transform(points1.begin(), points1.end(), std::back_inserter(intersectionCurve),
		[this, surface1, minHeight](const IntersectionPoint& p)
		{
			auto point = m_Sampler->GetPoint(*surface1, p.m_Parameter.x, p.m_Parameter.y);
			point.y = max(point.y - DETAIL_SPHERICAL_TOOL_RADIUS, minHeight);

			return point;
		}
	);

	if (intersectionCurve.size() < 2)
	{
		return result;
	}

	result.emplace_back(intersectionCurve.front().x, safeHeight, intersectionCurve.front().z);

	std::copy(intersectionCurve.begin(), intersectionCurve.end(), std::back_inserter(result));

	result.emplace_back(intersectionCurve.back().x, safeHeight, intersectionCurve.back().z);

	return result;
}

auto DetailSphericalPathGenerator::GeneratePathsForScrewButtonIntersection(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto result = std::vector<dxm::Vector3>();

	auto safeHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 2.0f;
	auto minHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_StandHeight;

	auto surface1 = std::find_if(m_OffsetSurfaces.begin(), m_OffsetSurfaces.end(),
		[](std::shared_ptr<SceneObjectCAD> object)
		{
			auto& tag = object->m_Tag;
			return tag.find("Screw") != std::string::npos;
		}
	);

	auto surface2 = std::find_if(m_OffsetSurfaces.begin(), m_OffsetSurfaces.end(),
		[](std::shared_ptr<SceneObjectCAD> object)
		{
			auto& tag = object->m_Tag;
			return tag.find("Button") != std::string::npos;
		}
	);

	if (surface1 == m_OffsetSurfaces.end() || surface2 == m_OffsetSurfaces.end())
	{
		return result;
	}

	auto startingPoints = std::vector<dxm::Vector3>{ { -2.0f, 3.0f, 0.0f }, { 2.0f, 3.0f, 0.0f } };

	for (auto& point : startingPoints)
	{
		auto intersectionParameters = IntersectionAlgorithmParameters();
		intersectionParameters.m_CursorPosition = point;
		intersectionParameters.m_UseCursorAsStartingPoint = true;
		intersectionParameters.m_PointDistance = 0.002f;

		auto intersectionAlgorithm = MultiIntersectionAlgorithm(intersectionParameters, *surface1, *surface2);

		auto intersection = intersectionAlgorithm.Find();

		auto converter = IntersectionResultConverter();

		auto [points1, points2] = converter.ToPointVectors(intersection);

		auto intersectionCurve = std::vector<dxm::Vector3>();

		std::transform(points1.begin(), points1.end(), std::back_inserter(intersectionCurve),
			[this, surface1, minHeight](const IntersectionPoint& p)
			{
				auto point = m_Sampler->GetPoint(*surface1, p.m_Parameter.x, p.m_Parameter.y);
				point.y = max(point.y - DETAIL_SPHERICAL_TOOL_RADIUS, minHeight);

				return point;
			}
		);

		if (intersectionCurve.size() < 2)
		{
			return result;
		}

		result.emplace_back(intersectionCurve.front().x, safeHeight, intersectionCurve.front().z);

		std::copy(intersectionCurve.begin(), intersectionCurve.end(), std::back_inserter(result));

		result.emplace_back(intersectionCurve.back().x, safeHeight, intersectionCurve.back().z);
	}

	return result;
}

auto DetailSphericalPathGenerator::GeneratePathsForSharpEdges(const MaterialBlockDetails& materialBlockDetails) -> std::vector<dxm::Vector3>
{
	auto safeHeight = materialBlockDetails.m_Position.y + materialBlockDetails.m_Size.y + 2.0f;

	auto result = std::vector<dxm::Vector3>();

	auto potentialSharpEdges = std::vector<std::shared_ptr<SceneObjectCAD>>();

	std::copy_if(m_OffsetSurfaces.begin(), m_OffsetSurfaces.end(), std::back_inserter(potentialSharpEdges),
		[](std::shared_ptr<SceneObjectCAD> object)
		{
			auto offset = std::dynamic_pointer_cast<OffsetSurface>(object);

			if (!offset)
			{
				return false;
			}

			auto inner = offset->InternalSurface();

			auto bezierC0 = std::dynamic_pointer_cast<BezierSurfaceC0>(inner.lock());

			return static_cast<bool>(bezierC0);
		}
	);

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
