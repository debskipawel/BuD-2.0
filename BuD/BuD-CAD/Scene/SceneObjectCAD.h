#pragma once

#include <BuD.h>

#include <Components/TransformComponent.h>

class AbstractVisitor;

class SceneObjectCAD
{
public:
	SceneObjectCAD(BuD::Scene& scene);

	virtual void Accept(AbstractVisitor& visitor) = 0;

	virtual ~SceneObjectCAD() = default;

	uint32_t Id() const { return m_SceneEntity.Id(); }

	BuD::SceneEntity m_SceneEntity;
	
	std::string m_Tag;

	bool m_Selected;
	TransformComponent m_Transform;
};
