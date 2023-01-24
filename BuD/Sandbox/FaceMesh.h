#pragma once

#include <Scene/Scene.h>
#include <Objects/Interfaces/IRenderable.h>
#include <Objects/Interfaces/IState.h>

class FaceMesh
{
public:
	FaceMesh(BuD::Scene& scene);

protected:
	BuD::SceneEntity m_Entity;
};
