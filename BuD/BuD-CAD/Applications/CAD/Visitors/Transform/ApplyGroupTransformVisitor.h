#pragma once

#include <Applications/CAD/Visitors/AbstractVisitor.h>

class ApplyGroupTransformVisitor : public AbstractVisitor
{
public:
	ApplyGroupTransformVisitor(const TransformComponent& additionalTransform, const dxm::Vector3& centroid);

	virtual void Visit(std::weak_ptr<SceneObjectCAD> object) override;

	virtual void Visit(Torus& torus) override;
	virtual void Visit(Point& point) override;

	virtual void SetInitialTransform(const TransformComponent& transform);

protected:
	std::set<uint32_t> m_ModifiedObjects;

	TransformComponent m_InitialTransform;
	TransformComponent m_AdditionalTransform;
	dxm::Vector3 m_Centroid;
};
