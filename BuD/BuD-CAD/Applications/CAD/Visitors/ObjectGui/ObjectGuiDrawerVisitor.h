#pragma once

#include <Applications/CAD/DataLayers/SceneDataLayer.h>
#include <Applications/CAD/Visitors/AbstractVisitor.h>

class ObjectGuiDrawerVisitor : public AbstractVisitor
{
public:
	ObjectGuiDrawerVisitor(SceneDataLayer& dataLayer);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(IntersectionCurve& curve) override;

	virtual void Visit(BezierCurveC0& curve) override;
	virtual void Visit(BezierCurveC2& curve) override;
	virtual void Visit(YukselInterpolatingCurveC2& curve) override;

	virtual void Visit(BezierPatchC0& patch) override;
	virtual void Visit(BezierPatchC2& patch) override;

	virtual void Visit(BezierSurfaceC0& surface) override;
	virtual void Visit(BezierSurfaceC2& surface) override;

protected:
	virtual void DrawGuiForSelectedTransform();
	
	virtual void DrawGuiForParameterSpace(ParameterizedObject2D& parameterized);

	virtual bool DrawGuiForTag(SceneObjectCAD& object);
	virtual bool DrawGuiForTransform(TransformComponent& transform);

	SceneDataLayer& m_SceneDataLayer;
};
