#pragma once

#include <BuD.h>

#include <Components/TransformComponent.h>

class AbstractVisitor;

class SceneObjectCAD
{
public:
	SceneObjectCAD(BuD::Scene& scene);
	virtual ~SceneObjectCAD();

	virtual void OnDelete() {}
	virtual void OnSelect() {}
	virtual void OnUnselect() {}

	virtual void Accept(AbstractVisitor& visitor) = 0;

	uint32_t Id() const { return m_SceneEntity.Id(); }

	BuD::SceneEntity m_SceneEntity;
	
	std::string m_Tag;

	bool m_Selected;
	TransformComponent m_Transform;
};
