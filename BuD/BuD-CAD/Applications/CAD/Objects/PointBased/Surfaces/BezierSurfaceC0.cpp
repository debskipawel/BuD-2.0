#include "BezierSurfaceC0.h"

#include <numbers>

#include <Applications/CAD/Objects/PointBased/Surfaces/BezierPatchC0.h>
#include <Applications/CAD/Visitors/AbstractVisitor.h>

BezierSurfaceC0::BezierSurfaceC0(SceneCAD& scene, std::vector<std::shared_ptr<BaseBezierPatch>> patches, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: BaseBezierSurface(scene, sizeU, sizeV, cylinder)
{
	m_BezierPatches = patches;
}

BezierSurfaceC0::BezierSurfaceC0(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: BaseBezierSurface(scene, sizeU, sizeV, cylinder)
{
	m_Tag = std::format("C0 Bezier surface {}", Id());

	if (cylinder)
	{
		auto controlPoints = CreateControlPointsForCylinder(scene, position, sizeU, sizeV);

		auto pointsCountU = m_SizeU * 3;

		for (int i = 0; i < m_SizeV; i++)
		{
			for (int j = 0; j < m_SizeU; j++)
			{
				std::vector<std::weak_ptr<Point>> patchControlPoints(16);

				auto startPointU = j * 3;
				auto startPointV = i * 3;

				for (int point = 0; point < 16; point++)
				{
					auto pointU = (startPointU + point % 4) % pointsCountU;
					auto pointV = startPointV + point / 4;

					auto pointIndex = pointV * pointsCountU + pointU;

					patchControlPoints[point] = controlPoints[pointIndex];
				}

				auto rangeU = dxm::Vector2{ static_cast<float>(j) / m_SizeU, static_cast<float>(j + 1) / m_SizeU };
				auto rangeV = dxm::Vector2{ static_cast<float>(i) / m_SizeV, static_cast<float>(i + 1) / m_SizeV };

				auto patch = std::make_shared<BezierPatchC0>(scene.m_Scene, patchControlPoints, rangeU, rangeV);

				for (auto& point : patchControlPoints)
				{
					auto pointShared = point.lock();
					pointShared->m_PointBasedObjects.push_back(patch);
				}

				m_BezierPatches.push_back(patch);
			}
		}
	}
	else
	{
		auto controlPoints = CreateControlPointsForFlatSurface(scene, position, sizeU, sizeV);

		auto pointsCountU = m_SizeU * 3 + 1;

		for (int i = 0; i < m_SizeV; i++)
		{
			for (int j = 0; j < m_SizeU; j++)
			{
				std::vector<std::weak_ptr<Point>> patchControlPoints(16);

				auto startPointU = j * 3;
				auto startPointV = i * 3;

				for (int point = 0; point < 16; point++)
				{
					auto pointU = startPointU + point % 4;
					auto pointV = startPointV + point / 4;

					auto pointIndex = pointV * pointsCountU + pointU;

					patchControlPoints[point] = controlPoints[pointIndex];
				}

				auto rangeU = dxm::Vector2{ static_cast<float>(j) / m_SizeU, static_cast<float>(j + 1) / m_SizeU };
				auto rangeV = dxm::Vector2{ static_cast<float>(i) / m_SizeV, static_cast<float>(i + 1) / m_SizeV };

				auto patch = std::make_shared<BezierPatchC0>(scene.m_Scene, patchControlPoints, rangeU, rangeV);

				for (auto& point : patchControlPoints)
				{
					auto pointShared = point.lock();
					pointShared->m_PointBasedObjects.push_back(patch);
				}

				m_BezierPatches.push_back(patch);
			}
		}
	}
}

void BezierSurfaceC0::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void BezierSurfaceC0::AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve)
{
	ParameterizedObject2D::AddIntersectionCurve(intersectionCurve);

	if (!m_IntersectionCurves.empty())
	{
		for (auto& patch : m_BezierPatches)
		{
			patch->SwitchToTrimmed();
		}
	}
}

void BezierSurfaceC0::RemoveIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve)
{
	ParameterizedObject2D::RemoveIntersectionCurve(intersectionCurve);

	if (m_IntersectionCurves.empty())
	{
		for (auto& patch : m_BezierPatches)
		{
			patch->SwitchToInstanced();
		}
	}
}

std::vector<std::weak_ptr<Point>> BezierSurfaceC0::CreateControlPointsForFlatSurface(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV)
{
	auto controlPoints = std::vector<std::weak_ptr<Point>>();

	auto patchWidthU = 1.0f;
	auto patchWidthV = 1.0f;

	auto startPosition = position - m_SizeU * patchWidthU / 2 * dxm::Vector3::UnitX - m_SizeV * patchWidthV / 2 * dxm::Vector3::UnitZ;
	auto pointStepU = patchWidthU / 3 * dxm::Vector3::UnitX;
	auto pointStepV = patchWidthV / 3 * dxm::Vector3::UnitZ;

	auto pointsCountU = m_SizeU * 3 + 1;
	auto pointsCountV = m_SizeV * 3 + 1;

	for (int i = 0; i < pointsCountV; i++)
	{
		for (int j = 0; j < pointsCountU; j++)
		{
			auto position = startPosition + i * pointStepV + j * pointStepU;
			auto point = scene.CreatePoint(position);

			controlPoints.push_back(std::dynamic_pointer_cast<Point>(point.lock()));
		}
	}

	return controlPoints;
}

std::vector<std::weak_ptr<Point>> BezierSurfaceC0::CreateControlPointsForCylinder(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV)
{
	auto controlPoints = std::vector<std::weak_ptr<Point>>();

	auto patchWidthU = 1.0f;
	auto patchWidthV = 1.0f;
	auto cylinderRadius = (m_SizeU * patchWidthU) / (2.0f * std::numbers::pi);

	auto cylinderMainAxis = dxm::Vector3::UnitZ;

	auto pointStepV = patchWidthV / 3 * cylinderMainAxis;

	float patchPivotAngle = 2.0f * std::numbers::pi / m_SizeU;

	auto ca = cosf(patchPivotAngle), sa = sinf(patchPivotAngle);
	auto scaleFactor = 4.0f / 3.0f * tanf(0.25f * patchPivotAngle);

	auto radiusVector = cylinderRadius * dxm::Vector3::UnitY;

	for (int i = 0; i < 3 * m_SizeV + 1; i++)
	{
		auto startingPosition = position - 0.5f * m_SizeV * cylinderMainAxis + i * pointStepV;

		for (int j = 0; j < m_SizeU; j++)
		{
			auto previousRadiusVector = radiusVector;
			auto tangent = cylinderMainAxis.Cross(previousRadiusVector);
			auto tangentNormalized = tangent;
			tangentNormalized.Normalize();

			auto nextRadiusVector = ca * previousRadiusVector + sa * tangent;
			auto nextTangent = cylinderMainAxis.Cross(nextRadiusVector);
			nextTangent.Normalize();

			auto position0 = startingPosition + previousRadiusVector;
			auto position1 = position0 + scaleFactor * tangentNormalized;
			auto position2 = startingPosition + nextRadiusVector - scaleFactor * nextTangent;

			auto point0 = scene.CreatePoint(position0);
			auto point1 = scene.CreatePoint(position1);
			auto point2 = scene.CreatePoint(position2);

			controlPoints.push_back(std::dynamic_pointer_cast<Point>(point0.lock()));
			controlPoints.push_back(std::dynamic_pointer_cast<Point>(point1.lock()));
			controlPoints.push_back(std::dynamic_pointer_cast<Point>(point2.lock()));

			radiusVector = nextRadiusVector;
		}
	}

	return controlPoints;
}
