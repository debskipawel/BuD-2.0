#include "BezierSurfaceC2.h"

#include <numbers>

#include <Visitors/AbstractVisitor.h>

BezierSurfaceC2::BezierSurfaceC2(SceneCAD& scene, std::vector<std::weak_ptr<BaseBezierPatch>> patches, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: BaseBezierSurface(scene, sizeU, sizeV, cylinder)
{
	m_BezierPatches = patches;
}

BezierSurfaceC2::BezierSurfaceC2(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: BaseBezierSurface(scene, sizeU, sizeV, cylinder)
{
	m_Tag = std::format("C2 Bezier surface {}", Id());

	if (cylinder)
	{
		auto controlPoints = CreateControlPointsForCylinder(scene, position, sizeU, sizeV);

		auto pointsCountU = m_SizeU;

		for (int i = 0; i < m_SizeV; i++)
		{
			for (int j = 0; j < m_SizeU; j++)
			{
				std::vector<std::weak_ptr<Point>> patchControlPoints(16);

				auto startPointU = j;
				auto startPointV = i;

				for (int point = 0; point < 16; point++)
				{
					auto pointU = (startPointU + point % 4) % pointsCountU;
					auto pointV = startPointV + point / 4;

					auto pointIndex = pointV * pointsCountU + pointU;

					patchControlPoints[point] = controlPoints[pointIndex];
				}

				auto patch = scene.CreateBezierPatchC2(patchControlPoints);
				auto patchShared = std::dynamic_pointer_cast<BaseBezierPatch>(patch.lock());

				m_BezierPatches.push_back(patchShared);
			}
		}
	}
	else
	{
		auto controlPoints = CreateControlPointsForFlatSurface(scene, position, sizeU, sizeV);

		auto pointsCountU = m_SizeU + 3;

		for (int i = 0; i < m_SizeV; i++)
		{
			for (int j = 0; j < m_SizeU; j++)
			{
				std::vector<std::weak_ptr<Point>> patchControlPoints(16);

				auto startPointU = j;
				auto startPointV = i;

				for (int point = 0; point < 16; point++)
				{
					auto pointU = startPointU + point % 4;
					auto pointV = startPointV + point / 4;

					auto pointIndex = pointV * pointsCountU + pointU;

					patchControlPoints[point] = controlPoints[pointIndex];
				}

				auto patch = scene.CreateBezierPatchC2(patchControlPoints);
				auto patchShared = std::dynamic_pointer_cast<BaseBezierPatch>(patch.lock());

				m_BezierPatches.push_back(patchShared);
			}
		}
	}
}

void BezierSurfaceC2::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

std::vector<std::weak_ptr<Point>> BezierSurfaceC2::CreateControlPointsForFlatSurface(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV)
{
	auto controlPoints = std::vector<std::weak_ptr<Point>>();

	auto uAxis = dxm::Vector3::UnitX;
	auto vAxis = dxm::Vector3::UnitZ;

	auto patchWidthU = 1.0f;
	auto patchWidthV = 1.0f;

	auto startPosition = position - (m_SizeU + 2) * patchWidthU / 2 * uAxis - (m_SizeV + 2) * patchWidthV / 2 * vAxis;
	auto pointStepU = patchWidthU * uAxis;
	auto pointStepV = patchWidthV * vAxis;

	auto pointsCountU = m_SizeU + 3;
	auto pointsCountV = m_SizeV + 3;

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

std::vector<std::weak_ptr<Point>> BezierSurfaceC2::CreateControlPointsForCylinder(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV)
{
	auto controlPoints = std::vector<std::weak_ptr<Point>>();

	auto patchWidthU = 1.0f;
	auto patchWidthV = 1.0f;
	auto cylinderRadius = (m_SizeU * patchWidthU) / (2.0f * std::numbers::pi);

	auto cylinderMainAxis = dxm::Vector3::UnitZ;

	auto pointStepV = patchWidthV * cylinderMainAxis;

	auto patchPivotAngle = 2.0f * std::numbers::pi_v<float> / m_SizeU;

	auto ca = cosf(patchPivotAngle), sa = sinf(patchPivotAngle);

	auto pointsCountU = m_SizeU;
	auto pointsCountV = m_SizeV + 3;

	auto radiusVector = cylinderRadius * dxm::Vector3::UnitY;

	for (int i = 0; i < pointsCountV; i++)
	{
		auto startPosition = position + (i - 0.5f * pointsCountV) * pointStepV;

		for (int j = 0; j < pointsCountU; j++)
		{
			auto point = scene.CreatePoint(startPosition + radiusVector);
			controlPoints.push_back(std::dynamic_pointer_cast<Point>(point.lock()));

			auto tangent = cylinderMainAxis.Cross(radiusVector);

			radiusVector = ca * radiusVector + sa * tangent;
		}
	}

	return controlPoints;
}