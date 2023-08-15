#pragma once

#include <Visitors/AbstractVisitor.h>

class ApplyGroupTransformVisitor : public AbstractVisitor
{
public:
	ApplyGroupTransformVisitor(const TransformComponent& initialTransform, const TransformComponent& additionalTransform, const dxm::Vector3& centroid);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void Visit(BezierCurveC0& curve) override;

protected:

	TransformComponent m_InitialTransform;
	TransformComponent m_AdditionalTransform;
	dxm::Vector3 m_Centroid;
};
