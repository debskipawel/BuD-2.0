#pragma once

#include <BuD.h>

class AbstractVisitor;

class SceneObjectCAD
{
public:
	SceneObjectCAD(BuD::Scene& scene);

	virtual void Accept(AbstractVisitor& visitor) = 0;

	virtual ~SceneObjectCAD() = default;

	BuD::SceneEntity m_SceneEntity;
	std::string m_Tag;
};
