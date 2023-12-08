#pragma once

#include <Applications/CAD/Scene/SceneObjectCAD.h>

#include <Applications/CAD/Objects/Point.h>
#include <Applications/CAD/Objects/Torus.h>
#include <Applications/CAD/Objects/InfinitePlane.h>

#include <Applications/CAD/Objects/Intersection/IntersectionCurve.h>

#include <Applications/CAD/Objects/PointBased/Curve/BezierCurveC0.h>
#include <Applications/CAD/Objects/PointBased/Curve/BezierCurveC2.h>
#include <Applications/CAD/Objects/PointBased/Curve/YukselInterpolatingCurveC2.h>

#include <Applications/CAD/Objects/PointBased/Surfaces/BezierSurfaceC0.h>
#include <Applications/CAD/Objects/PointBased/Surfaces/BezierSurfaceC2.h>
#include <Applications/CAD/Objects/PointBased/Surfaces/BezierPatchC0.h>
#include <Applications/CAD/Objects/PointBased/Surfaces/BezierPatchC2.h>

#include <Applications/CAD/Objects/PointBased/Surfaces/OffsetSurface.h>

class AbstractVisitor
{
public:
	virtual ~AbstractVisitor() = default;

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object);

	virtual void Visit(Point& point) {}
	virtual void Visit(Torus& torus) {}

	virtual void Visit(InfinitePlane& plane) {}

	virtual void Visit(IntersectionCurve& curve) {}

	virtual void Visit(BezierCurveC0& curve) {}
	virtual void Visit(BezierCurveC2& curve) {}
	virtual void Visit(YukselInterpolatingCurveC2& curve) {}

	virtual void Visit(BezierPatchC0& patch) {}
	virtual void Visit(BezierPatchC2& patch) {}
	
	virtual void Visit(BezierSurfaceC0& surface) {}
	virtual void Visit(BezierSurfaceC2& surface) {}

	virtual void Visit(OffsetSurface& surface) {}

protected:
	std::weak_ptr<SceneObjectCAD> m_Caller;
};
