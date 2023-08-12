#pragma once

#include <Visitors/AbstractVisitor.h>

class ApplyTransformVisitor : public AbstractVisitor
{
public:
	ApplyTransformVisitor(const TransformComponent& originalTransform, const TransformComponent& additionalTransform, const dxm::Vector3& centroid);

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

protected:

	TransformComponent m_OriginalTransform;
	TransformComponent m_AdditionalTransform;
	dxm::Vector3 m_Centroid;
};
