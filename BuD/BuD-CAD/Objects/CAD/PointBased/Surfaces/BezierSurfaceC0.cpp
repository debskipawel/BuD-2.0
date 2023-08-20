#include "BezierSurfaceC0.h"

#include <numbers>

#include <Objects/CAD/PointBased/Surfaces/BezierPatchC0.h>
#include <Visitors/AbstractVisitor.h>

BezierSurfaceC0::BezierSurfaceC0(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, bool cylinder)
	: BaseBezierSurface(scene.m_Scene, sizeU, sizeV, cylinder)
{
	m_Tag = std::format("C0 Bezier surface {}", Id());

	if (cylinder)
	{
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

				auto nextRadiusVector = ca * previousRadiusVector + sa * tangent;
				auto nextTangent = cylinderMainAxis.Cross(nextRadiusVector);

				auto position0 = startingPosition + previousRadiusVector;
				auto position1 = position0 + scaleFactor * tangent;
				auto position2 = startingPosition + nextRadiusVector - scaleFactor * nextTangent;

				auto point0 = scene.CreatePoint(position0);
				auto point1 = scene.CreatePoint(position1);
				auto point2 = scene.CreatePoint(position2);

				m_ControlPoints.push_back(std::dynamic_pointer_cast<Point>(point0.lock()));
				m_ControlPoints.push_back(std::dynamic_pointer_cast<Point>(point1.lock()));
				m_ControlPoints.push_back(std::dynamic_pointer_cast<Point>(point2.lock()));

				radiusVector = nextRadiusVector;
			}
		}

		auto pointsCountU = m_SizeU * 3;

		for (int i = 0; i < m_SizeV; i++)
		{
			for (int j = 0; j < m_SizeU; j++)
			{
				std::vector<std::weak_ptr<Point>> controlPoints(16);

				auto startPointU = j * 3;
				auto startPointV = i * 3;

				for (int point = 0; point < 16; point++)
				{
					auto pointU = (startPointU + point % 4) % pointsCountU;
					auto pointV = startPointV + point / 4;

					auto pointIndex = pointV * pointsCountU + pointU;

					controlPoints[point] = m_ControlPoints[pointIndex];
				}

				auto patch = scene.CreateBezierPatchC0(controlPoints);
				auto patchShared = std::dynamic_pointer_cast<BaseBezierPatch>(patch.lock());

				m_BezierPatches.push_back(patchShared);

				for (auto& controlPoint : controlPoints)
				{
					auto pointShared = controlPoint.lock();
					pointShared->m_PointBasedObjects.push_back(patchShared);
				}
			}
		}
	}
	else
	{
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

				m_ControlPoints.push_back(std::dynamic_pointer_cast<Point>(point.lock()));
			}
		}

		for (int i = 0; i < m_SizeV; i++)
		{
			for (int j = 0; j < m_SizeU; j++)
			{
				std::vector<std::weak_ptr<Point>> controlPoints(16);

				auto startPointU = j * 3;
				auto startPointV = i * 3;

				for (int point = 0; point < 16; point++)
				{
					auto pointU = startPointU + point % 4;
					auto pointV = startPointV + point / 4;

					auto pointIndex = pointV * pointsCountU + pointU;

					controlPoints[point] = m_ControlPoints[pointIndex];
				}

				auto patch = scene.CreateBezierPatchC0(controlPoints);
				auto patchShared = std::dynamic_pointer_cast<BaseBezierPatch>(patch.lock());

				m_BezierPatches.push_back(patchShared);

				for (auto& controlPoint : controlPoints)
				{
					auto pointShared = controlPoint.lock();
					pointShared->m_PointBasedObjects.push_back(patchShared);
				}
			}
		}
	}
}

void BezierSurfaceC0::Accept(AbstractVisitor& visitor)
{
	visitor.Visit(*this);
}

void BezierSurfaceC0::OnPointModify()
{
	for (auto& patch : m_BezierPatches)
	{
		auto patchShared = patch.lock();

		if (!patchShared)
		{
			continue;
		}

		patchShared->OnPointModify();
	}
}
