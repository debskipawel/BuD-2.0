#pragma once

#include <BuD.h>

#include <Applications/CAD/Components/TransformComponent.h>

class AbstractVisitor;

class SceneObjectCAD
{
public:
	SceneObjectCAD(BuD::Scene& scene);
	virtual ~SceneObjectCAD();

	virtual void Accept(AbstractVisitor& visitor) = 0;

	uint32_t Id() const { return m_SceneEntity.Id(); }
	
	std::string m_Tag;
	TransformComponent m_Transform;

protected:
	BuD::SceneEntity m_SceneEntity;
};
