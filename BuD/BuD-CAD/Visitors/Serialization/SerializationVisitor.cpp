#include "SerializationVisitor.h"

#include <Serializer.h>

void SerializationVisitor::Visit(Torus& torus)
{
	auto& scene = MG1::Scene::Get();
	auto t = MG1::Torus();

	auto position = torus.m_Transform.m_Position;
	auto rotation = torus.m_Transform.m_Rotation;
	auto scale = torus.m_Transform.m_Scale;

	t.position = MG1::Float3{ position.x, position.y, position.z };
	t.rotation = MG1::Float3{ rotation.x, rotation.y, rotation.z };
	t.scale = MG1::Float3{ scale.x, scale.y, scale.z };

	t.name = torus.m_Tag;
	t.largeRadius = torus.m_InstanceData.m_OuterRadius;
	t.smallRadius = torus.m_InstanceData.m_InnerRadius;
	t.samples = MG1::Uint2{ static_cast<uint32_t>(16), static_cast<uint32_t>(8) };

	t.SetId(torus.Id());

	scene.tori.push_back(t);
}

void SerializationVisitor::Visit(Point& point)
{
	auto& scene = MG1::Scene::Get();
	auto p = MG1::Point();

	p.name = point.m_Tag;
	p.SetId(point.Id());

	auto position = point.m_Transform.m_Position;

	p.position = MG1::Float3{ position.x, position.y, position.z };

	scene.points.push_back(p);
}

void SerializationVisitor::Visit(BezierCurveC0& curve)
{
	auto& scene = MG1::Scene::Get();
	auto bezierCurve = MG1::BezierC0();

	bezierCurve.name = curve.m_Tag;
	bezierCurve.SetId(curve.Id());

	for (auto controlPoint : curve.m_ControlPoints)
	{
		auto controlPointShared = controlPoint.lock();

		if (!controlPointShared)
		{
			continue;
		}

		bezierCurve.controlPoints.push_back(MG1::PointRef(controlPointShared->Id()));
	}

	scene.bezierC0.push_back(bezierCurve);
}

void SerializationVisitor::Visit(BezierCurveC2& curve)
{
	auto& scene = MG1::Scene::Get();
	auto bezierCurve = MG1::BezierC2();

	bezierCurve.name = curve.m_Tag;
	bezierCurve.SetId(curve.Id());

	for (auto controlPoint : curve.m_ControlPoints)
	{
		auto controlPointShared = controlPoint.lock();

		if (!controlPointShared)
		{
			continue;
		}

		bezierCurve.controlPoints.push_back(MG1::PointRef(controlPointShared->Id()));
	}

	scene.bezierC2.push_back(bezierCurve);
}

void SerializationVisitor::Visit(YukselInterpolatingCurveC2& curve)
{
	auto& scene = MG1::Scene::Get();
	auto bezierCurve = MG1::InterpolatedC2();

	bezierCurve.name = curve.m_Tag;
	bezierCurve.SetId(curve.Id());

	for (auto controlPoint : curve.m_ControlPoints)
	{
		auto controlPointShared = controlPoint.lock();

		if (!controlPointShared)
		{
			continue;
		}

		bezierCurve.controlPoints.push_back(MG1::PointRef(controlPointShared->Id()));
	}

	scene.interpolatedC2.push_back(bezierCurve);
}

void SerializationVisitor::Visit(BezierPatchC0& patch)
{
}

void SerializationVisitor::Visit(BezierPatchC2& patch)
{
}

void SerializationVisitor::Visit(BezierSurfaceC0& surface)
{
	auto& scene = MG1::Scene::Get();
	auto bezierSurface = MG1::BezierSurfaceC0();

	bezierSurface.name = surface.m_Tag;
	bezierSurface.SetId(surface.Id());

	bezierSurface.uWrapped = surface.m_Cylinder;
	bezierSurface.vWrapped = false;
	bezierSurface.size = MG1::Uint2{ surface.m_SizeU, surface.m_SizeV };

	for (auto& patch : surface.m_BezierPatches)
	{
		auto patchShared = patch.lock();

		if (!patchShared)
		{
			continue;
		}

		auto bezierPatch = MG1::BezierPatchC0();
		bezierPatch.name = patchShared->m_Tag;
		bezierPatch.SetId(patchShared->Id());

		bezierPatch.samples = MG1::Uint2{ static_cast<uint32_t>(16), static_cast<uint32_t>(16) };

		for (auto& controlPoint : patchShared->m_ControlPoints)
		{
			auto controlPointShared = controlPoint.lock();

			if (!controlPointShared)
			{
				continue;
			}

			bezierPatch.controlPoints.push_back(MG1::PointRef(controlPointShared->Id()));
		}

		bezierSurface.patches.push_back(bezierPatch);
	}

	scene.surfacesC0.push_back(bezierSurface);
}

void SerializationVisitor::Visit(BezierSurfaceC2& surface)
{
	auto& scene = MG1::Scene::Get();
	auto bezierSurface = MG1::BezierSurfaceC2();

	bezierSurface.name = surface.m_Tag;
	bezierSurface.SetId(surface.Id());

	bezierSurface.uWrapped = surface.m_Cylinder;
	bezierSurface.vWrapped = false;
	bezierSurface.size = MG1::Uint2{ surface.m_SizeU, surface.m_SizeV };

	for (auto& patch : surface.m_BezierPatches)
	{
		auto patchShared = patch.lock();

		if (!patchShared)
		{
			continue;
		}

		auto bezierPatch = MG1::BezierPatchC2();
		bezierPatch.name = patchShared->m_Tag;
		bezierPatch.SetId(patchShared->Id());

		bezierPatch.samples = MG1::Uint2{ static_cast<uint32_t>(16), static_cast<uint32_t>(16) };

		for (auto& controlPoint : patchShared->m_ControlPoints)
		{
			auto controlPointShared = controlPoint.lock();

			if (!controlPointShared)
			{
				continue;
			}

			bezierPatch.controlPoints.push_back(MG1::PointRef(controlPointShared->Id()));
		}

		bezierSurface.patches.push_back(bezierPatch);
	}

	scene.surfacesC2.push_back(bezierSurface);
}
