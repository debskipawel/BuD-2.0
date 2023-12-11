#include "SceneCAD.h"

#include <Applications/CAD/Objects/Torus.h>
#include <Applications/CAD/Objects/Point.h>

#include <Applications/CAD/Objects/FinitePlane.h>
#include <Applications/CAD/Objects/InfinitePlane.h>

#include <Applications/CAD/Objects/ParameterizedObject2D.h>

#include <Applications/CAD/Objects/Intersection/IntersectionCurve.h>

#include <Applications/CAD/Objects/PointBased/Curve/BezierCurveC0.h>
#include <Applications/CAD/Objects/PointBased/Curve/BezierCurveC2.h>
#include <Applications/CAD/Objects/PointBased/Curve/YukselInterpolatingCurveC2.h>

#include <Applications/CAD/Objects/PointBased/Surfaces/BezierPatchC0.h>
#include <Applications/CAD/Objects/PointBased/Surfaces/BezierPatchC2.h>

#include <Applications/CAD/Objects/PointBased/Surfaces/BezierSurfaceC0.h>
#include <Applications/CAD/Objects/PointBased/Surfaces/BezierSurfaceC2.h>

SceneCAD::SceneCAD()
	: m_Scene(), m_ObjectList()
{
	m_Grid = std::make_unique<Grid>(m_Scene);
	m_MainCursor = std::make_shared<Cursor>(m_Scene);

	m_CentroidCursor = std::make_shared<Cursor>(m_Scene);
	m_CentroidCursor->SetPosition({ NAN, NAN, NAN });
}

SceneCAD::~SceneCAD()
{
	m_ObjectList.clear();
}

void SceneCAD::Clear()
{
	m_ObjectList.clear();
}

void SceneCAD::DeleteObject(SceneObjectCAD& object)
{
	auto id = object.Id();

	if (!m_ObjectList.contains(id))
	{
		return;
	}

	auto& objectPtr = m_ObjectList.at(id);

	m_ObjectList.erase(id);
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateTorus(dxm::Vector3 position)
{
	auto torus = std::make_shared<Torus>(m_Scene, position);
	m_ObjectList.emplace(torus->Id(), torus);

	BuD::Log::WriteInfo("Successfully created a torus.");

	return torus;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreatePoint(dxm::Vector3 position)
{
	auto point = std::make_shared<Point>(m_Scene, position);
	m_ObjectList.emplace(point->Id(), point);

	BuD::Log::WriteInfo("Successfully created a point.");

	return point;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateFinitePlane(dxm::Vector3 position, dxm::Vector3 dU, dxm::Vector3 dV, float widthU, float widthV)
{
	auto plane = std::make_shared<FinitePlane>(m_Scene, position, dU, dV, widthU, widthV);
	m_ObjectList.emplace(plane->Id(), plane);

	BuD::Log::WriteInfo("Successfully created a finite plane.");

	return plane;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateFinitePlane(dxm::Vector3 position, dxm::Vector3 normal, float widthU, float widthV)
{
	auto plane = std::make_shared<FinitePlane>(m_Scene, position, normal, widthU, widthV);
	m_ObjectList.emplace(plane->Id(), plane);

	BuD::Log::WriteInfo("Successfully created a finite plane.");

	return plane;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateInfinitePlane(dxm::Vector3 position, dxm::Vector3 dU, dxm::Vector3 dV)
{
	auto plane = std::make_shared<InfinitePlane>(m_Scene, position, dU, dV);
	m_ObjectList.emplace(plane->Id(), plane);

	BuD::Log::WriteInfo("Successfully created an infinite plane.");

	return plane;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateInfinitePlane(dxm::Vector3 position, dxm::Vector3 normal)
{
	auto plane = std::make_shared<InfinitePlane>(m_Scene, position, normal);
	m_ObjectList.emplace(plane->Id(), plane);

	BuD::Log::WriteInfo("Successfully created an infinite plane.");

	return plane;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateBezierCurveC0(std::vector<std::weak_ptr<Point>> controlPoints)
{
	auto curve = std::make_shared<BezierCurveC0>(m_Scene, controlPoints);
	m_ObjectList.emplace(curve->Id(), curve);

	for (auto& controlPoint : controlPoints)
	{
		auto controlPointShared = controlPoint.lock();
		controlPointShared->m_PointBasedObjects.push_back(curve);
	}

	BuD::Log::WriteInfo("Successfully created a Bezier C0 curve.");

	return curve;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateBezierCurveC2(std::vector<std::weak_ptr<Point>> controlPoints)
{
	auto curve = std::make_shared<BezierCurveC2>(m_Scene, controlPoints);
	m_ObjectList.emplace(curve->Id(), curve);

	for (auto& controlPoint : controlPoints)
	{
		auto controlPointShared = controlPoint.lock();
		controlPointShared->m_PointBasedObjects.push_back(curve);
	}

	BuD::Log::WriteInfo("Successfully created a Bezier C2 curve.");

	return curve;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateYukselInterpolatingCurveC2(std::vector<std::weak_ptr<Point>> controlPoints)
{
	auto curve = std::make_shared<YukselInterpolatingCurveC2>(m_Scene, controlPoints);
	m_ObjectList.emplace(curve->Id(), curve);

	for (auto& controlPoint : controlPoints)
	{
		auto controlPointShared = controlPoint.lock();
		controlPointShared->m_PointBasedObjects.push_back(curve);
	}

	BuD::Log::WriteInfo("Successfully created a Yuksel interpolating C2 curve.");

	return curve;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateFlatBezierSurfaceC0(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV, float patchSizeU, float patchSizeV)
{
	auto surface = std::make_shared<BezierSurfaceC0>(*this, position, patchesU, pachesV, patchSizeU, patchSizeV, false);
	surface->UpdateOwnership();

	m_ObjectList.emplace(surface->Id(), surface);

	BuD::Log::WriteInfo("Successfully created a flat C0 Bezier surface.");

	return surface;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateCylinderBezierSurfaceC0(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV, float patchSizeU, float patchSizeV)
{
	auto surface = std::make_shared<BezierSurfaceC0>(*this, position, patchesU, pachesV, patchSizeU, patchSizeV, true);
	surface->UpdateOwnership();

	m_ObjectList.emplace(surface->Id(), surface);

	BuD::Log::WriteInfo("Successfully created a cylinder C0 Bezier surface.");

	return surface;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateFlatBezierSurfaceC2(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV, float patchSizeU, float patchSizeV)
{
	auto surface = std::make_shared<BezierSurfaceC2>(*this, position, patchesU, pachesV, patchSizeU, patchSizeV, false);
	surface->UpdateOwnership();

	m_ObjectList.emplace(surface->Id(), surface);

	BuD::Log::WriteInfo("Successfully created a flat C2 Bezier surface.");

	return surface;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateCylinderBezierSurfaceC2(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV, float patchSizeU, float patchSizeV)
{
	auto surface = std::make_shared<BezierSurfaceC2>(*this, position, patchesU, pachesV, patchSizeU, patchSizeV, true);
	surface->UpdateOwnership();

	m_ObjectList.emplace(surface->Id(), surface);

	BuD::Log::WriteInfo("Successfully created a cylinder C2 Bezier surface.");

	return surface;
}

std::weak_ptr<SceneObjectCAD> SceneCAD::CreateIntersectionCurve(std::weak_ptr<SceneObjectCAD> surface, std::vector<IntersectionPoint> intersectionPoints)
{
	auto surfaceShared = std::dynamic_pointer_cast<ParameterizedObject2D>(surface.lock());

	if (!surfaceShared)
	{
		return std::weak_ptr<SceneObjectCAD>();
	}

	auto curve = std::make_shared<IntersectionCurve>(m_Scene, surface, intersectionPoints);
	m_ObjectList.emplace(curve->Id(), curve);

	surfaceShared->AddIntersectionCurve(curve);

	BuD::Log::WriteInfo("Successfully created an intersection curve.");

	return curve;
}
